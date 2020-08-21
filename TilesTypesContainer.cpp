#include "TilesTypesContainer.h"

TilesTypesContainer::TilesTypesContainer()
{
	add("GrassTile");
	add("BrickTile");
	add("WaterTile");
}

void TilesTypesContainer::add(std::string tileType)
{
	tiles.push_back(std::make_shared<TileChosenTextureType>());
	if (tileType == "GrassTile") tiles.back()->setTextureType(std::make_shared<GrassTile>());
	else if (tileType == "BrickTile") tiles.back()->setTextureType(std::make_shared<BrickTile>());
	else if (tileType == "WaterTile") tiles.back()->setTextureType(std::make_shared<WaterTile>());
}

const sf::Texture& TilesTypesContainer::getTexturedTileRandomly()
{
	auto tileNumber = randomGenerator.generateRandomInt(0, static_cast<int>(tiles.size() - 1));
	return tiles[tileNumber]->getTexturedTile();
}

const sf::Texture& TilesTypesContainer::getTexturedTileRandomlyButNotGrassType()
{
	auto tileNumber = randomGenerator.generateRandomInt(1, static_cast<int>(tiles.size() - 1));
	return tiles[tileNumber]->getTexturedTile();
}
