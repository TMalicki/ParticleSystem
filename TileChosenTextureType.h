#pragma once
#include "GrassTile.h"
#include "BrickTile.h"
#include "WaterTile.h"

class TileChosenTextureType
{
private:
	std::shared_ptr<TilesTextureBody> chosenTextureTypeForTile;
public:
	TileChosenTextureType() : chosenTextureTypeForTile{} {}

	void setTextureType(const std::shared_ptr<TilesTextureBody>& type) { chosenTextureTypeForTile = type; }
	const sf::Texture& getTexturedTile() { return chosenTextureTypeForTile->getTexturedTile(); }
};
