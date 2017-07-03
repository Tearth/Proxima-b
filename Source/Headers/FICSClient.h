#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <thread>
#include <ws2tcpip.h>
#include <winsock2.h>

#include "Headers/Console.h"
#include "Headers/CoordsConverter.h"

using namespace std;

class FICSClient
{
public:
	FICSClient();
	~FICSClient();

	void Init(Console* console, string userName, string password, string seek);

	//Starts network thread
	void ThreadStart();

	//Stopps network thread
	void ThreadStop();

	//Sends command to the server
	int NetworkWrite(const char* input);

	FICSMode Mode;

	string EnemyMove;
	string AIMove;
	EColor InitialColor;
	bool LoopGames;
private:
	bool networkInit(char* address, char* port);
	void networkRun();
	void networkLoop();
	void networkStop();

	thread networkLoopThread;
	bool endThread;

	WSAData _wsaData;
	SOCKET _socket;
	Console* _console;

	string _userName;
	string _password;
	string _seek;
};