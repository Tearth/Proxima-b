#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Headers/TextureRecord.h"

using namespace std;

class TexturesLoader
{
public:
	TexturesLoader();
	~TexturesLoader();

	bool LoadFromFile(string fileName);
	vector<TextureRecord> GetAllTextures();
	shared_ptr<sf::Texture> GetTextureByName(string name);
private:
	vector<TextureRecord> _loadedTextures;
};