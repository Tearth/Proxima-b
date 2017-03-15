#include "Headers/Position.h"

Position::Position()
{
	X = 0;
	Y = 0;
}

Position::Position(const Position &p)
{
	X = p.X;
	Y = p.Y;
}

Position::Position(char x, char y)
{
	X = x;
	Y = y;
}

Position::Position(sf::Vector2f pos)
{
	X = pos.x;
	Y = pos.y;
}

Position::~Position()
{

}

Position Position::operator+(const Position &p)
{
	return Position(X + p.X, Y + p.Y);
}

Position Position::operator-(const Position &p)
{
	return Position(X - p.X, Y - p.Y);
}

Position Position::operator-()
{
	return Position(-X, -Y);
}

Position Position::operator*(int c)
{
	return Position(X * c, Y * c);
}

bool Position::operator==(const Position &p)
{
	return X == p.X && Y == p.Y;
}

bool Position::operator!=(const Position &p)
{
	return X != p.X || Y != p.Y;
}

string Position::ToString()
{
	return to_string(X) + "-" + to_string(Y);
}