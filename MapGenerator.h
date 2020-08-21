#pragma once
#include <iostream>
#include <vector>
#include "RandomNumberGenerator.h"
#include <SFML/Graphics.hpp>

// that should be strategy pattern!!!

class MapGenerator
{
protected: //? is it necessary?
	const unsigned int tileAmountInWidth;
	const unsigned int tileAmountInHeight;
	std::vector<std::vector<int>> level;

private:
public:
	MapGenerator(const sf::Vector2u tileAmount) : tileAmountInWidth{ tileAmount.x }, tileAmountInHeight{ tileAmount.y }
	{
		generateMap();
	}
	virtual ~MapGenerator() {};
	virtual void generateDungeonWithAlgorithm() = 0;

	void generateMap();
	std::vector<std::vector<int>> getTilesToRender() { return level; };
};