#include "LoadWorld.h"

void LoadWorld::loadWorld()
{
	actualLevel->generateDungeonWithAlgorithm();
	prepareTheWorld();
	generateWorld();
}

void LoadWorld::prepareTheWorld()
{
	auto temp = actualLevel->getTilesToRender();
	std::vector<std::shared_ptr<MapTile>> tempTiles{};

	for (size_t i = 0; i < temp.size(); i++)
	{
		mapTiles.push_back(tempTiles);
		for (size_t j = 0; j < temp[i].size(); j++)
		{
			mapTiles[i].push_back(std::make_shared<MapTile>(mapTile.getTileSize()));
		}
	}
}

void LoadWorld::generateWorld()
{
	std::vector<std::vector<int>> tilesToRender = actualLevel->getTilesToRender();

	for (size_t i = 0; i < tilesToRender.size(); i++)
	{
		for (size_t j = 0; j < tilesToRender[i].size(); j++)
		{
			if (tilesToRender[i][j] == 1)
			{
				mapTiles[i][j]->generateWalkableTile(static_cast<int>(j), static_cast<int>(i));
			}
			else mapTiles[i][j]->generateNewTileButNotWalkable(static_cast<int>(j), static_cast<int>(i));
		}
	}
	notify();
}

void LoadWorld::detach(ObserverInterface* observer)
{
	auto iterator = std::find(listObserver.begin(), listObserver.end(), observer);
	if (iterator != listObserver.end())
	{
		listObserver.erase(iterator); // memory leak
	}
}
