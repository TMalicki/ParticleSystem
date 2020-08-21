#pragma once
#include "TileSettings.h"
#include "MapSettings.h"

class GameSettings
{
private:
	TileSettings tileSettings;
	MapSettings mapSettings;

	const unsigned int windowWidth;
	const unsigned int windowHeight;

public:
	GameSettings(unsigned int tileWidth = 40, unsigned int tileHeight = 40, unsigned int tileAmountHorizon = 20, unsigned int tileAmountVertical = 16)
		: tileSettings(tileWidth, tileHeight), mapSettings(tileAmountHorizon, tileAmountVertical) 
		,windowWidth{ tileWidth * tileAmountHorizon }, windowHeight{ tileHeight * tileAmountVertical } { }

	std::shared_ptr<GameSettings> getSettings() { return std::make_shared<GameSettings>(*this); }
	const sf::Vector2u getTileSize() { return tileSettings.getTileSize(); }
	const sf::Vector2u getTileAmount() { return mapSettings.getTileAmount(); }
};