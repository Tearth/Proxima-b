#include "Headers/Console.h"

Console::Console() : 
	_newCommand(ECommandType::None, vector<string>())
{
	
}

Console::~Console()
{

}

void Console::Init(int maxLines, int maxInputLength, sf::Vector2f position, sf::Vector2f size)
{
	_maxLines = maxLines;
	_maxInputLength = maxInputLength;

	_font.loadFromFile("Fonts/lucon.ttf");

	_consoleBackground.setSize(size);
	_consoleBackground.setOutlineColor(sf::Color::Green);
	_consoleBackground.setOutlineThickness(1);
	_consoleBackground.setFillColor(sf::Color::Black);
	_consoleBackground.setPosition(position);

	int textBoxHeight = 22;
	_textBoxArea.setSize(sf::Vector2f(size.x, textBoxHeight));
	_textBoxArea.setOutlineColor(sf::Color::Green);
	_textBoxArea.setOutlineThickness(1);
	_textBoxArea.setFillColor(sf::Color::Black);
	_textBoxArea.setPosition(sf::Vector2f(position.x, position.y + size.y - textBoxHeight));

	_textBoxContent.setFont(_font);
	_textBoxContent.setCharacterSize(10);
	_textBoxContent.setFillColor(sf::Color::White);
	_textBoxContent.setPosition(_textBoxArea.getPosition() + sf::Vector2f(5, 5));

	_consoleContent.setFont(_font);
	_consoleContent.setCharacterSize(10);
	_consoleContent.setFillColor(sf::Color::White);
	_consoleContent.setPosition(_consoleBackground.getPosition() + sf::Vector2f(5, 5));

}

string Console::getConsoleContent()
{
	string content;
	for (int i = 0; i < _consoleLines.size(); i++)
	{
		content += _consoleLines[i] + "\n";
	}

	return content;
}

void Console::Draw(sf::RenderWindow* window)
{
	_consoleContent.setString(getConsoleContent());

	window->draw(_consoleBackground);
	window->draw(_textBoxArea);
	window->draw(_textBoxContent);
	window->draw(_consoleContent);
}

void Console::KeyPressed(sf::Event::TextEvent key)
{
	std::string text = _textBoxContent.getString();

	//Backspace
	if (key.unicode == 8)
	{
		if(text.size() > 0)
			text.pop_back();
	}
	//Enter
	else if (key.unicode == 13)
	{
		parseCommand();
		return;
	}
	else if (key.unicode < 128)
	{
		if(text.size() < _maxInputLength)
			text += key.unicode;
	}

	_textBoxContent.setString(text);
}

void Console::parseCommand()
{
	std::string text = _textBoxContent.getString();
	vector<string> splitted = split(text, ' ');

	if (splitted.size() == 0)
		return;

	string commandText = tolowerString(splitted[0]);
	ECommandType commandType = ECommandType::None;

	if      (commandText == "hello" || commandText == "hi") commandType = ECommandType::Hello;
	else if (commandText == "addpiece") commandType = ECommandType::AddPiece;
	else if (commandText == "save") commandType = ECommandType::Save;
	else if (commandText == "load") commandType = ECommandType::Load;
	else if (commandText == "heur" || commandText == "heuristic") commandType = ECommandType::Heuristic;
	else if (commandText == "minmax" || commandText == "n") commandType = ECommandType::MinMax;
	else if (commandText == "hash" || commandText == "gethash") commandType = ECommandType::GetHash;
	else if (commandText == "mate" || commandText == "ismate") commandType = ECommandType::IsMate;
	else if (commandText == "markpos" || commandText == "mpos") commandType = ECommandType::MarkPosition;
	else if (commandText == "domove" || commandText == "dm") commandType = ECommandType::DoMove;
	else if (commandText == "setmaxtimeai" || commandText == "maxtime") commandType = ECommandType::SetMaxTimeAI;
	else if (commandText == "setgamemode" || commandText == "gamemode" || commandText == "mode") commandType = ECommandType::SetGameMode;
	else if (commandText == "stopfics") commandType = ECommandType::StopFICS;
	else if (commandText == "resetboard" || commandText == "reset" ) commandType = ECommandType::ResetBoard;
	else if (commandText == "help" || commandText == "h") commandType = ECommandType::Help;
	else if (commandText == "sendfics" || commandText == "sendficscommand") commandType = ECommandType::SendFICSCommand;
	else if (commandText == "ficsloop") commandType = ECommandType::FICSLoop;
	else return;

	vector<string> args;
	if (splitted.size() > 1)
	{
		for (int i = 1; i < splitted.size(); i++)
			args.push_back(splitted[i]);
	}

	Command cmd(commandType, args);

	_newCommand = cmd;

	AddNewLine(" > " + text);
	_textBoxContent.setString("");
}

bool Console::IsNewCommandAvailable()
{
	if (_newCommand.Type != ECommandType::None)
		return true;
	return false;
}

void Console::AddNewLine(string line)
{
	if (_consoleLines.size() >= _maxLines)
		_consoleLines.erase(_consoleLines.begin());
	_consoleLines.push_back(line);
}

Command Console::GetNewCommand()
{
	if (IsNewCommandAvailable())
	{
		Command tmpCmd = _newCommand;
		_commandsHistory.push_back(tmpCmd);
		_newCommand.Type = ECommandType::None;
		return tmpCmd;
	}

	return Command(ECommandType::None, std::vector<string>());
}