#include "Headers/TexturesLoader.h"

TexturesLoader::TexturesLoader()
{

}

TexturesLoader::~TexturesLoader()
{

}

bool TexturesLoader::LoadFromFile(string fileName)
{
	ifstream dataFile;
	dataFile.open(fileName, ifstream::in);

	if (!dataFile.good())
		return false;

	string line;
	while (getline(dataFile, line))
	{
		shared_ptr<sf::Texture> texture(new sf::Texture());
		texture->loadFromFile("Textures/" + line + ".png");

		TextureRecord texRecord(texture, line);
		_loadedTextures.push_back(texRecord);
	}
	dataFile.close();

	return true;
}

vector<TextureRecord> TexturesLoader::GetAllTextures()
{
	return _loadedTextures;
}

shared_ptr<sf::Texture> TexturesLoader::GetTextureByName(string name)
{
	for (int i = 0; i < _loadedTextures.size(); i++)
	{
		if (_loadedTextures[i].Name == name)
			return _loadedTextures[i].Texture;
	}

	throw runtime_error(std::string("Texture not found: ") + name);
}