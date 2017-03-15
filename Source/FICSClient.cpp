#include "Headers/FICSClient.h"

FICSClient::FICSClient()
{
	LoopGames = false;
	endThread = true;
}

FICSClient::~FICSClient()
{

}

void FICSClient::networkLoop()
{
	Mode = FICSMode::LoginSequence;

	while (!endThread)
	{
		Sleep(100);

		char buffer[4096];
		int bSize = sizeof(buffer);
		
		memset(buffer, 0, bSize);

		int inDataLength = recv(_socket, buffer, bSize, 0);
		int nError = WSAGetLastError();

		if (inDataLength == -1)
		{
			if (Mode == FICSMode::WaitingForEnemyMove ||
				Mode == FICSMode::WaitingForAIMove)
				continue;
		}

		if (nError != WSAEWOULDBLOCK && nError != 0)
		{
			_console->AddNewLine("Winsock error");
			break;
		}

		if (inDataLength > 0)
			std::cout << buffer;

		string bufferStr = string(buffer); 
		if (LoopGames && (bufferStr.find("Blitz rating adjustment") != string::npos ||
			bufferStr.find("Standard rating adjustment") != string::npos ||
			bufferStr.find("No rating adjustment") != string::npos))
			Mode = FICSMode::Reset;

		if (Mode == FICSMode::LoginConfirm)
		{
			if (bufferStr.find("Starting FICS session as") != string::npos &&
				bufferStr.find("is a registered name") != string::npos)
			{
				_console->AddNewLine("Login success");
				Mode = FICSMode::Seek;
			}
		}
		else if (Mode == FICSMode::LoginSequence)
		{
			_console->AddNewLine("Logging...");

			NetworkWrite(string(_userName + "\n").c_str());
			NetworkWrite(string(_password + "\n").c_str());
			Mode = FICSMode::LoginConfirm;
		}
		else if (Mode == FICSMode::Seek)
		{
			_console->AddNewLine("Seek new game");

			NetworkWrite(string(_seek + "\n").c_str());
			Mode = FICSMode::SeekWaiting;
		}
		else if (Mode == FICSMode::SeekWaiting)
		{
			if (bufferStr.find("accepts your seek") != string::npos ||
				bufferStr.find("accepts the match offer") != string::npos ||
				bufferStr.find("seek matches one already posted by") != string::npos)
			{
				_console->AddNewLine("Seek accepted!");

				if (bufferStr.find("Creating: proximab") != string::npos)
				{
					_console->AddNewLine("Selected color: white");
					Mode = FICSMode::WaitingForAIMove;
					InitialColor = EColor::White;
				}
				else
				{
					_console->AddNewLine("Selected color: black");
					Mode = FICSMode::WaitingForEnemyMove;
					InitialColor = EColor::Black;
				}
			}
		}
		else if (Mode == FICSMode::AIMove)
		{
			NetworkWrite(string(AIMove + "\n").c_str());
			Mode = FICSMode::WaitingForEnemyMove;
		}
		else if (Mode == FICSMode::WaitingForEnemyMove)
		{
			int startIndex = bufferStr.find("<12>", 0);
			int endIndex = bufferStr.find("\n", startIndex);

			if (startIndex != -1 && endIndex != -1)
			{
				string line = bufferStr.substr(startIndex, endIndex - startIndex);
				vector<string> splittedLine = split(line, ' ');

				if (startIndex != -1)
				{
					string verifyColor = InitialColor == EColor::White ? "W" : "B";
					if (splittedLine[9] == verifyColor)
					{
						EnemyMove = splittedLine[27];
						Mode = FICSMode::EnemyMove;
					}
				}
			}
		}
	}
}

void FICSClient::Init(Console* console, string userName, string password, string seek)
{
	_console = console;
	_userName = userName;
	_password = password;
	_seek = seek;
}

void FICSClient::ThreadStart()
{
	endThread = false;

	networkLoopThread = thread(&FICSClient::networkRun, this);
	networkLoopThread.detach();
}

void FICSClient::ThreadStop()
{
	endThread = true;
}

void FICSClient::networkRun()
{
	_console->AddNewLine("Start FICS init");
	_console->AddNewLine("freechess.org:5000");

	bool initResult = networkInit("freechess.org", "5000");
	if (!initResult)
	{
		_console->AddNewLine("FICS init failed.");
		return;
	}

	_console->AddNewLine("Start FICS loop");
	networkLoop();

	_console->AddNewLine("Stop FICS");
	networkStop();
}

bool FICSClient::networkInit(char* address, char* port)
{
	int result = WSAStartup(MAKEWORD(2, 2), &_wsaData);
	if (result != 0) {
		return false;
	}

	struct addrinfo *addrResult = NULL,
		*ptr = NULL,
		hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo(address, port, &hints, &addrResult);
	if (result != 0) {
		return false;
	}

	for (ptr = addrResult; ptr != NULL; ptr = ptr->ai_next) {
		_socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (_socket == INVALID_SOCKET) {
			return false;
		}

		result = connect(_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (result == SOCKET_ERROR) {
			closesocket(_socket);
			_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	if (_socket == INVALID_SOCKET) {
		return false;
	}

	unsigned long on = 1;
	if (0 != ioctlsocket(_socket, FIONBIO, &on))
		return false;

	return true;
}

int FICSClient::NetworkWrite(const char* input)
{
	string sendMsg = " >>> " + string(input);
	cout << sendMsg;

	return send(_socket, input, (int)strlen(input), 0);
}

void FICSClient::networkStop()
{
	closesocket(_socket);
	WSACleanup();
}