#pragma once

#include "RandomNumberGenerator.h"
#include "TileChosenTextureType.h"

//context class - user of strategy design pattern (TileTypes interface class)
class TilesTypesContainer
{
private:
	RandomNumberGenerator randomGenerator;
	std::vector<std::shared_ptr<TileChosenTextureType>> tiles;
public:
	TilesTypesContainer();

	void add(std::string tileType);
	const sf::Texture& getTexturedTileRandomly();
	const sf::Texture& getTexturedTileRandomlyButNotGrassType();
	const sf::Texture& getGrassTileTexture() { return tiles[0]->getTexturedTile(); }
};
