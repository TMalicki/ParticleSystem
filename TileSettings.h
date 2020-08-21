#pragma once
#include <SFML/Graphics.hpp>

class TileSettings
{
protected:
	const unsigned int tileWidth;
	const unsigned int tileHeight;

public:
	TileSettings(unsigned int tileWidth = 40, unsigned int tileHeight = 40) : tileWidth{ tileWidth }, tileHeight{ tileHeight } {}

	const sf::Vector2u getTileSize() {
		return (sf::Vector2u{ tileWidth, tileHeight }); };
};