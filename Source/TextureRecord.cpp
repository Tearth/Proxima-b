#include "Headers/TextureRecord.h"

TextureRecord::TextureRecord(shared_ptr<sf::Texture> texture, string name)
{
	Texture = texture;
	Name = name;
}

TextureRecord::~TextureRecord()
{

}