#pragma once
#include "Headers/Window.h"
#include "Headers/Command.h"
#include "Headers/Split.h"
#include "Headers/Enums.h"

class Console
{
public:
	Console();
	~Console();

	void Init(int maxLines, int maxInputLength, sf::Vector2f position, sf::Vector2f size);
	void Draw(sf::RenderWindow* window);

	//Handles key press
	void KeyPressed(sf::Event::TextEvent key);

	//Returns true if any command is available to parse
	bool IsNewCommandAvailable();

	//Returns first available command
	Command GetNewCommand();

	//Writes line to the console
	void AddNewLine(string line);
private:
	void parseCommand();
	string getConsoleContent();

	sf::RectangleShape _consoleBackground;
	sf::RectangleShape _textBoxArea;
	sf::Font _font;
	sf::Text _textBoxContent;
	sf::Text _consoleContent;

	int _maxLines;
	int _maxInputLength;

	Command _newCommand;
	vector<Command> _commandsHistory;
	vector<string> _consoleLines;
};