#pragma once

#include <iostream>
#include "TilesTextureBody.h"

class BrickTile : public TilesTextureBody
{
private:

public:
	BrickTile() : TilesTextureBody()
	{
		loadTexture();
	};

	virtual void loadTexture()
	{
		if (!tileSetTexture.loadFromFile("tilesTextures/dirtTile.png"))
		{
			std::cout << "Texture was not loaded.";
		}
	}
};