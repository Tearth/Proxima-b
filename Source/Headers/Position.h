#pragma once
#include <SFML/Graphics.hpp>

using namespace std;

class Position
{
public:
	Position();
	Position(const Position &p);
	Position(char x, char y);
	Position(sf::Vector2f pos);

	~Position();

	Position operator+(const Position &p);
	Position operator-(const Position &p);
	Position operator-();
	Position operator*(int c);
	bool operator==(const Position &p);
	bool operator!=(const Position &p);

	string ToString();

	char X;
	char Y;
};