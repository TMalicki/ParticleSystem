#pragma once

#include "TilesTypesContainer.h"

class MapTile
{
private:
	TilesTypesContainer tilesTypeContainer;
	sf::Sprite tileSprite;
	sf::Texture tileTexture;
	sf::Vector2f position;
	sf::Vector2u tileSize;
public:
	MapTile(sf::Vector2u tileSize) : position{}, tilesTypeContainer{}, tileSize{ tileSize } { }
	
	void setSpriteScale();
	void setSpritePosition(int posX, int posY);
	const MapTile& generateNewTile(int posX, int posY);
	const MapTile& generateNewTileButNotWalkable(int posX, int posY);
	const MapTile& generateWalkableTile(int posX, int posY);
	const sf::Sprite& getSprite() { return tileSprite; }
	const sf::Vector2u& getTileSize() { return tileSize; }
};
