#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

using namespace std;

class TextureRecord
{
public:
	TextureRecord(shared_ptr<sf::Texture> texture, string name);
	~TextureRecord();

	shared_ptr<sf::Texture> Texture;
	string Name;

private:
};