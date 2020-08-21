#pragma once

#include <iostream>
#include "TilesTextureBody.h"

class WaterTile : public TilesTextureBody
{
private:

public:
	WaterTile() : TilesTextureBody() 
	{
		loadTexture();
	};

	virtual void loadTexture()
	{
		if (!tileSetTexture.loadFromFile("tilesTextures/waterTile.png"))
		{
			std::cout << "Texture was not loaded.";
		}
	}
};