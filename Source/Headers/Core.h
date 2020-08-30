#pragma once
#include "Headers/Window.h"
#include "Headers/Board.h"
#include "Headers/TexturesLoader.h"
#include "Headers/SpriteRecord.h"
#include "Headers/PiecesManager.h"
#include "Headers/Console.h"
#include "Headers/AI.h"
#include "Headers/CoordsConverter.h"
#include "Headers/Random.h"
#include "Headers/FICSClient.h"
#include "Headers/ConfigLoader.h"

class Core
{
public:
	Core();
	~Core();

	void Init();
	void Run();
private:
	void loadSprites();
	void addSprite(string spriteName, string textureName);
	void draw();
	void logic();
	void parseEvent(sf::Event event);
	void parseConsole();
	void drawBoard();
	void saveBoard(string fileName);
	void loadBoard(string fileName);
	sf::Sprite GetSpriteByName(string name);
	sf::Vector2f getPositionByBoardCoords(Position pos);
	unsigned long long perft(Board board, AI ai, EColor color, int depth);

	AI _ai;
	Window _window;
	Board _board;
	TexturesLoader _texturesLoader;
	Console _console;
	
	int _fieldWidthHeight;
	Position _selectedField;
	sf::Vector2f _startBoardPosition;
	vector<SpriteRecord> _sprites;
	vector<Position> _availableMoves;

	GameMode _gameMode;
	EColor _currentAIColor;
	bool _redrawAfterMove;

	FICSClient _ficsClient;
	ConfigLoader _configLoader;
};