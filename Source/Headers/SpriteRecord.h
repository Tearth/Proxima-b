#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class SpriteRecord
{
public:
	SpriteRecord(string name, sf::Sprite sprite);
	~SpriteRecord();

	string Name;
	sf::Sprite Sprite;
};