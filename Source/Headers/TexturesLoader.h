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

	//Loads texture from specified file name
	bool LoadFromFile(string fileName);

	//Returns an vector of loaded textures
	vector<TextureRecord> GetAllTextures();

	//Returns texture with specified name
	shared_ptr<sf::Texture> GetTextureByName(string name);
private:
	vector<TextureRecord> _loadedTextures;
};