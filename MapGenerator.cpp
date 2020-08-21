#include "MapGenerator.h"

void MapGenerator::generateMap()
{
	std::vector<int> temp{};
	for (size_t i = 0; i < tileAmountInWidth; i++)
	{
		temp.push_back(0);
	}

	for (size_t j = 0; j < tileAmountInHeight; j++)
	{
		level.push_back(temp);
	}
}
