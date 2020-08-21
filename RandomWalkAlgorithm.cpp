#include "RandomWalkAlgorithm.h"

void RandomWalkAlgorithm::generateSpawnPointRandomly()
{
	spawnPoint = sf::Vector2u{ static_cast<unsigned int>((randomNumber.generateRandomInt(0, tileAmountInWidth - 1))),
							static_cast<unsigned int>((randomNumber.generateRandomInt(0, tileAmountInHeight - 1))) };
}

void RandomWalkAlgorithm::getWalkDirection()
{
	do
	{
		auto temp = randomNumber.generateRandomInt(0, 3);
		switch (temp)
		{
		case 0:
		{
			walkDirection = DirectionOfWalk::Up;
			randomDirection = upDirection;
		}
		break;
		case 1:
		{
			walkDirection = DirectionOfWalk::Right;
			randomDirection = rightDirection;
		}
		break;
		case 2:
		{
			walkDirection = DirectionOfWalk::Down;
			randomDirection = downDirection;
		}
		break;
		case 3:
		{
			walkDirection = DirectionOfWalk::Left;
			randomDirection = leftDirection;
		}
		break;
		}
	} while (randomDirection == lastDirection /*|| randomDirection == -lastDirection*/);
}

void RandomWalkAlgorithm::getRandomMaxLength()
{
	maxLength = randomNumber.generateRandomInt(1, tileAmountInWidth);
	actualLength = 0;
}

void RandomWalkAlgorithm::moveInDirection(sf::Vector2u& startingPoint, sf::Vector2u direction)
{
	startingPoint += direction;
}

bool RandomWalkAlgorithm::checkBorderCollision()
{
	if ((actualCoord.y <= 0 && randomDirection == upDirection)
		|| (actualCoord.y >= tileAmountInHeight - 1 && randomDirection == downDirection)
		|| (actualCoord.x >= tileAmountInWidth - 1 && randomDirection == rightDirection)
		|| (actualCoord.x <= 0 && randomDirection == leftDirection))
		return true;
	else return false;
}

void RandomWalkAlgorithm::buildDungeonTunnel()
{
	getWalkDirection();
	getRandomMaxLength();

	while (maxLength > actualLength)
	{
		if (checkBorderCollision() == true)	break;
		else
		{
			level[actualCoord.y][actualCoord.x] = 1;
			moveInDirection(actualCoord, randomDirection);
			actualLength++;
		}
	}

	if (actualLength >= 1)
	{
		lastDirection = randomDirection;
		maxTunnels--;
	}
}

void RandomWalkAlgorithm::generateDungeonWithAlgorithm()
{
	generateSpawnPointRandomly();
	actualCoord = spawnPoint;
	while (maxTunnels >= 1) buildDungeonTunnel();

	/*
	for (size_t i = 0; i < level.size(); i++)
	{
		for (size_t j = 0; j < level[i].size(); j++)
		{
			std::cout << level[i][j];
		}
		std::cout << "\n";
	}
	*/
}
