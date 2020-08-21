#pragma once
#include <SFML/Graphics.hpp>
#include "TileSettings.h"

class TilesTextureBody
{
protected:
	sf::Texture tileSetTexture;

public:
	TilesTextureBody() : tileSetTexture{} { }
	virtual ~TilesTextureBody() {};
	virtual void loadTexture() = 0;

	const sf::Texture& getTexturedTile() { return tileSetTexture; }
};

