#include "Headers/Core.h"

Core::Core()
{
	_currentAIColor = EColor::White;
	_redrawAfterMove = false;
}

Core::~Core()
{
	PiecesManager::Delete();
}

void Core::Init()
{
	_configLoader.LoadFromFile("config.txt");

	Random::Init();
	PiecesManager::Init();

	_gameMode = GameMode::None;
	_window.Init("Szachy", 800, 600);
	_board.InitDefaultSettings();
	_texturesLoader.LoadFromFile("Textures/data.txt");
	_console.Init(48, 36, sf::Vector2f(550, 60), sf::Vector2f(230, 520));
	
	_selectedField = Position(-1, -1);
	_startBoardPosition = sf::Vector2f(50, 60);
	_fieldWidthHeight = 60;
	_ai.Init(&_console);

	_ficsClient.Init(&_console, _configLoader.GetValueOfKey("FICS_username"),
								_configLoader.GetValueOfKey("FICS_password"),
								_configLoader.GetValueOfKey("FICS_seek"));

	loadSprites();
}

void Core::loadSprites()
{
	addSprite("Field1", "field1");
	addSprite("Field2", "field2");
	addSprite("Selected", "selected");

	addSprite("Pawn white", "pawn_w");
	addSprite("Rook white", "rook_w");
	addSprite("Knight white", "knight_w");
	addSprite("Bishop white", "bishop_w");
	addSprite("Queen white", "queen_w");
	addSprite("King white", "king_w");

	addSprite("Pawn black", "pawn_b");
	addSprite("Rook black", "rook_b");
	addSprite("Knight black", "knight_b");
	addSprite("Bishop black", "bishop_b");
	addSprite("Queen black", "queen_b");
	addSprite("King black", "king_b");
}

void Core::addSprite(string spriteName, string textureName)
{
	_sprites.push_back(SpriteRecord(spriteName, sf::Sprite(*_texturesLoader.GetTextureByName(textureName))));
}

sf::Sprite Core::GetSpriteByName(string name)
{
	for (int i = 0; i < _sprites.size(); i++)
	{
		if (_sprites[i].Name == name)
			return _sprites[i].Sprite;
	}

	throw runtime_error(std::string("Sprite not found: ") + name);
}

void Core::Run()
{
	while (_window.Get()->isOpen())
	{
		sf::Event event;
		while (_window.Get()->pollEvent(event))
		{
			parseEvent(event);
		}

		if(!_redrawAfterMove)
			logic();
		_redrawAfterMove = false;

		_window.Get()->clear();
		draw();
		_window.Get()->display();
	}
}

void Core::draw()
{
	drawBoard();
	
	_console.Draw(_window.Get());
}

void Core::logic()
{
	switch (_gameMode)
	{
	case(GameMode::AIvsPlayer):
	{
		if (_currentAIColor == EColor::Black)
		{
			Move bestMove = _ai.GetBestMove(_board, _currentAIColor);
			if (bestMove.From == Position() && bestMove.To == Position())
			{
				_console.AddNewLine("Mate");
			}
			else
			{
				_availableMoves.push_back(bestMove.From);
				_availableMoves.push_back(bestMove.To);
				_board.DoMove(bestMove);
			}
			_currentAIColor = inverseColor(_currentAIColor);
		}
		break;
	}
	case(GameMode::AIvsAI):
	{
		Move bestMove = _ai.GetBestMove(_board, _currentAIColor);
		if (bestMove.From == Position() && bestMove.To == Position())
		{
			_console.AddNewLine("Mate");
		}
		else
		{
			_availableMoves.clear();
			_availableMoves.push_back(bestMove.From);
			_availableMoves.push_back(bestMove.To);
			_board.DoMove(bestMove);
		}
		_currentAIColor = inverseColor(_currentAIColor);

		break;
	}
	case(GameMode::FICSvsAI):
	{
		CoordsConverter conv;

		if (_ficsClient.Mode == FICSMode::Reset)
		{
			_board.InitDefaultSettings();
			_ai.Reset();

			_ficsClient.Mode = FICSMode::Seek;
		}
		else if (_ficsClient.Mode == FICSMode::WaitingForAIMove)
		{
			Move bestMove = _ai.GetBestMove(_board, _ficsClient.InitialColor);

			string from = conv.CovertFromBoardToTrue(bestMove.From);
			string to = conv.CovertFromBoardToTrue(bestMove.To);
			string finalMove = from + "-" + to;

			if (bestMove.IsPromotion)
			{
				finalMove += "=" + toupper(bestMove.PromotionPiece);
			}

			_ficsClient.AIMove = finalMove;

			_availableMoves.clear();
			_availableMoves.push_back(bestMove.From);
			_availableMoves.push_back(bestMove.To);
			_board.DoMove(bestMove);

			_ficsClient.Mode = FICSMode::AIMove;
		}
		else if (_ficsClient.Mode == FICSMode::EnemyMove)
		{
			string enemyMove = _ficsClient.EnemyMove;
			string f1, f2;
			Move move;

			if (enemyMove == "o-o")
			{
				if(_ficsClient.InitialColor == EColor::White)
					move = Move(Position(4, 0), Position(6, 0));
				else
					move = Move(Position(4, 7), Position(6, 7));
			}
			else if (enemyMove == "o-o-o")
			{
				if (_ficsClient.InitialColor == EColor::White)
					move = Move(Position(4, 0), Position(2, 0));
				else
					move = Move(Position(4, 7), Position(2, 7));
			}
			else
			{
				CoordsConverter conv;

				if (enemyMove.length() == 5)
				{
					f1 = enemyMove.substr(0, 2);
					f2 = enemyMove.substr(3, 2);
				}
				else if (enemyMove.length() == 9)
				{
					f1 = enemyMove.substr(2, 2);
					f2 = enemyMove.substr(5, 2);

					move.IsPromotion = true;
					move.PromotionPiece = enemyMove.substr(8, 1)[0];

					if (_ficsClient.InitialColor == EColor::White)
						move.PromotionPiece = toupper(move.PromotionPiece);
					else
						move.PromotionPiece = tolower(move.PromotionPiece);
				}
				else
				{
					f1 = enemyMove.substr(2, 2);
					f2 = enemyMove.substr(5, 2);
				}

				move.From = conv.ConvertCoordinates(f1);
				move.To = conv.ConvertCoordinates(f2);
			}

			_availableMoves.clear();
			_availableMoves.push_back(move.From);
			_availableMoves.push_back(move.To);
			_board.DoMove(move);

			_ficsClient.Mode = FICSMode::WaitingForAIMove;
		}
		break;
	}
	}
}

void Core::parseEvent(sf::Event event)
{
	if (event.type == sf::Event::Closed)
		_window.Get()->close();
	else if (event.type == sf::Event::TextEntered)
		_console.KeyPressed(event.text);
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::Vector2f coords = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

		int fieldX = (coords.x - _startBoardPosition.x) / _fieldWidthHeight;
		int fieldY = (coords.y - _startBoardPosition.y) / _fieldWidthHeight;
		Position fieldPosition = Position(fieldX, fieldY);
		
		Position lastClickedPosition = _selectedField;
		if (coords.x < _startBoardPosition.x || coords.x > _startBoardPosition.x + (8 * _fieldWidthHeight) ||
			coords.y < _startBoardPosition.y || coords.y > _startBoardPosition.y + (8 * _fieldWidthHeight) ||
			fieldPosition == _selectedField)
		{
			_selectedField = Position(-1, -1);
		}
		else
		{
			_selectedField = fieldPosition;
		}

		char field = _board.GetFieldAtPosition(Position(_selectedField));
		PieceBase* selectedPiece = PiecesManager::GetPieceByID(field);
		if (selectedPiece != 0 && selectedPiece->GetID() == 'p')
			dynamic_cast<Pawn*>(selectedPiece)->SetEnPassantMoves(_board.EnPassantPositions);

		vector<Position> tmpAvailableMoves = _availableMoves;
		_availableMoves.clear();
		
		if(selectedPiece == 0 || _board.CanKill(lastClickedPosition, fieldPosition))
		{
			char lastClickedPiece = _board.GetFieldAtPosition(lastClickedPosition);
			EColor lastPieceColor = _board.GetColorByPieceID(lastClickedPiece);

			if (lastClickedPosition != Position(-1, -1) && lastClickedPiece != '0' && lastClickedPosition != fieldPosition)
			{
				Move move(lastClickedPosition, fieldPosition);
				bool moveAvailable = false;
				for (int i = 0; i < tmpAvailableMoves.size(); i++)
				{
					if (tmpAvailableMoves[i] == move.To)
					{
						moveAvailable = true;
						break;
					}
				}

				if (_gameMode == GameMode::AIvsPlayer && (!moveAvailable || lastPieceColor == EColor::Black))
				{
					_console.AddNewLine("Wrong move!");
				}
				else if (_gameMode == GameMode::AIvsPlayer &&
						(_ai.IsMate(_board, EColor::White) || _ai.IsMate(_board, EColor::Black)))
				{
					_console.AddNewLine("Mate. Can't do move.");
				}
				else
				{
					Board tmpBoard = _board;

					if (_gameMode == GameMode::AIvsPlayer && _currentAIColor == EColor::White)
					{
						if (lastClickedPiece == 'p' && move.To.Y == 0)
						{
							move.IsPromotion = true;
							move.PromotionPiece = 'q';
						}
					}

					tmpBoard.DoMove(move);

					if (_ai.IsCheck(tmpBoard, EColor::White))
					{
						_console.AddNewLine("Invalid move, white king is in check");
					}
					else
					{
						_board = tmpBoard;
						_selectedField = Position(-1, -1);

						if (_gameMode == GameMode::AIvsPlayer)
						{
							_currentAIColor = inverseColor(_currentAIColor);
							_redrawAfterMove = true;
						}
					}
				}
			}
			else
			{
				_availableMoves = _ai.GetFieldAttackers(_board, fieldPosition, EColor::Black);
			}
		}
		else if (selectedPiece != 0)
		{
			EColor pieceColor = _board.GetColorByPieceID(field);
			vector<Move> moves = selectedPiece->GetAvailableMoves(_board, _selectedField, pieceColor);
			
			if (field == 'k' || field == 'K')
				moves = _ai.CheckKingMoves(_board, moves, _selectedField);

			for (int i = 0; i < moves.size(); i++)
			{
				_availableMoves.push_back(moves[i].To);
			}
		}
	}

	parseConsole();
}

void Core::parseConsole()
{
	if (_console.IsNewCommandAvailable())
	{
		Command c = _console.GetNewCommand();
		
		switch (c.Type)
		{
		case(ECommandType::Hello): { _console.AddNewLine("Hi, how are you?"); break; }
		case(ECommandType::AddPiece):
		{
			if (c.Arguments.size() < 3)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			char pieceType = c.Arguments[0][0];
			if (!PiecesManager::IsPieceValid(pieceType))
			{
				_console.AddNewLine("Invalid piece type");
				break;
			}

			char x = c.Arguments[1][0];
			char y = c.Arguments[2][0];

			_board.Set(Position((x - '0') - 1, 8 - (y - '0')), pieceType);
			break;
		}
		case(ECommandType::Save):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			string fileName = c.Arguments[0];
			saveBoard(fileName);

			break;
		}
		case(ECommandType::Load):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			string fileName = c.Arguments[0];
			loadBoard(fileName);

			break;
		}
		case(ECommandType::Heuristic):
		{
			_ai.RunHeuristic(_board);
			break;
		}
		case(ECommandType::MinMax):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}
			EColor color = EColor::White;
			if (c.Arguments[0] == "b")
				color = EColor::Black;

			_console.AddNewLine("MinMax result: ");

			Move bestMove = _ai.GetBestMove(_board, color);

			if (bestMove.From == Position() && bestMove.To == Position())
			{
				_console.AddNewLine("Mate");
			}
			else
			{
				_board.DoMove(bestMove);
			}
			break;
		}
		case(ECommandType::GetHash):
		{
			unsigned long long int hash = _board.GetHash();
			_console.AddNewLine("Hash: " + to_string(hash));

			break;
		}
		case(ECommandType::IsMate):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			EColor color = EColor::White;
			if (c.Arguments[0] == "b")
				color = EColor::Black;

			bool isMate = _ai.IsMate(_board, color);
			_console.AddNewLine("Is mate: " + to_string(isMate));

			break;
		}
		case(ECommandType::MarkPosition):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}
			CoordsConverter conv; 
			Position pos = conv.ConvertCoordinates(c.Arguments[0]);
			_selectedField = pos;

			break;
		}
		case(ECommandType::DoMove):
		{
			if (c.Arguments.size() < 2)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			EColor color;
			string move;

			if (c.Arguments[0] == "w")
				color = EColor::White;
			else
				color = EColor::Black;

			move = c.Arguments[1];

			CoordsConverter conv;
			Move trueMove = conv.FromTrueToBoard(move, color, _board);
			_board.DoMove(trueMove);

			break;
		}
		case(ECommandType::SetMaxTimeAI):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			float time = stof(c.Arguments[0]);
			_ai.MaxTime = time;

			break;
		}
		case(ECommandType::SetGameMode):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			string mode = c.Arguments[0];
			if (mode == "fics")
			{
				_gameMode = GameMode::FICSvsAI;
				_ficsClient.ThreadStart();
			}
			else if (mode == "none") _gameMode = GameMode::None;
			else if (mode == "aivsai") _gameMode = GameMode::AIvsAI;
			else if (mode == "aivsplayer") _gameMode = GameMode::AIvsPlayer;
			else
			{
				_console.AddNewLine("Bad gamemode name! Available modes:");
				_console.AddNewLine(" - none");
				_console.AddNewLine(" - aivsai");
				_console.AddNewLine(" - aivsplayer");
				_console.AddNewLine(" - fics");
			}

			break;
		}
		case(ECommandType::StopFICS):
		{
			_ficsClient.ThreadStop();
			break;
		}
		case(ECommandType::ResetBoard):
		{
			_board.InitDefaultSettings();
			_ai.Reset();
			break;
		}
		case(ECommandType::FICSLoop):
		{
			if (c.Arguments.size() < 1)
			{
				_console.AddNewLine("Invalid parameters count");
				break;
			}

			string mode = c.Arguments[0];
			if (mode == "true" || mode == "t")
				_ficsClient.LoopGames = true;
			else if (mode == "false" || mode == "f")
				_ficsClient.LoopGames = false;
			else
				_console.AddNewLine("Invalid parameter (should be true/false or t/f");

			break;
		}
		case(ECommandType::SendFICSCommand):
		{
			string cmd;
			for (int i = 0; i < c.Arguments.size(); i++)
			{
				cmd += c.Arguments[i];
				cmd += " ";
			}
			cmd += "\n";

			int result = _ficsClient.NetworkWrite(cmd.c_str());

			break;
		}
		case(ECommandType::Help):
		{
			_console.AddNewLine("Available commands:");
			_console.AddNewLine(" Hello - say hello");
			_console.AddNewLine(" AddPiece - add piece to board");
			_console.AddNewLine(" Save - save board");
			_console.AddNewLine(" Load - load board");
			_console.AddNewLine(" Heuristic - get heuristic info");
			_console.AddNewLine(" MinMax - do minmax");
			_console.AddNewLine(" GetHash - get board hash");
			_console.AddNewLine(" IsMate - is mate");
			_console.AddNewLine(" MarkPosition - select field with pos");
			_console.AddNewLine(" DoMove - do move");
			_console.AddNewLine(" SetMaxTimeAI - set max time for minmax");
			_console.AddNewLine(" SetGameMode - set mode");
			_console.AddNewLine(" StopFICS - stop thread with fics");
			_console.AddNewLine(" ResetBoard - reset board to default");
			_console.AddNewLine(" SendFICS - send command to FICS");
			_console.AddNewLine(" FICSLoop - loop FICS games");
			_console.AddNewLine(" Help - you are here");
				
			break;
		}
		}
	}
}

void Core::drawBoard()
{
	sf::Sprite field1 = GetSpriteByName("Field1");
	sf::Sprite field2 = GetSpriteByName("Field2");
	sf::Sprite selectedSprite = GetSpriteByName("Selected");
	bool isWhite = false;

	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			sf::Vector2f fieldPosition = getPositionByBoardCoords(Position(x, y));
			if (isWhite)
			{
				field1.setPosition(fieldPosition);
				_window.Get()->draw(field1);
			}
			else
			{
				field2.setPosition(fieldPosition);
				_window.Get()->draw(field2);
			}
			isWhite = !isWhite;

			char currentField = _board.GetFieldAtPosition(Position(x, y));
			if (currentField != '0')
			{
				bool isBlack = isupper(currentField);
				EColor color = isBlack ? EColor::Black : EColor::White;

				string spriteName = PiecesManager::GetPieceByID(currentField)->GetSpriteName(color);
				sf::Sprite sprite = GetSpriteByName(spriteName);
				sprite.setPosition(fieldPosition);
				_window.Get()->draw(sprite);
			}

			if (_selectedField != Position(-1, -1) && _selectedField == Position(x, y))
			{
				selectedSprite.setPosition(fieldPosition);
				_window.Get()->draw(selectedSprite);
			}
		}

		isWhite = !isWhite;
	}

	for (int i = 0; i < _availableMoves.size(); i++)
	{
		sf::Vector2f fieldPosition = getPositionByBoardCoords(_availableMoves[i]);
		selectedSprite.setPosition(fieldPosition);
		_window.Get()->draw(selectedSprite);
	}
}

sf::Vector2f Core::getPositionByBoardCoords(Position pos)
{
	return _startBoardPosition + sf::Vector2f(pos.X * _fieldWidthHeight, pos.Y * _fieldWidthHeight);
}

void Core::saveBoard(string fileName)
{
	ofstream dataFile;
	dataFile.open("Boards/" + fileName + ".txt", ifstream::out);

	for (int x = 0; x < 8; x++)
	{
		for (int y = 0; y < 8; y++)
		{
			Position pos(x, y);
			char field = _board.GetFieldAtPosition(pos);
			dataFile << field;
		}
		dataFile << '\n';
	}
	dataFile.close();
}

void Core::loadBoard(string fileName)
{
	ifstream dataFile;
	dataFile.open("Boards/" + fileName + ".txt", ifstream::in);

	if (!dataFile.good())
	{
		_console.AddNewLine("Board " + fileName + " not found");
		return;
	}

	string line;
	int x = 0;
	while (getline(dataFile, line))
	{
		for (int y = 0; y < 8; y++)
		{
			_board.Set(Position(x, y), line[y]);
		}

		x++;
	}
	dataFile.close();
}