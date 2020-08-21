#pragma once
#include <SFML/Graphics.hpp>
#include "MapGenerator.h"

enum class DirectionOfWalk { Up, Right, Down, Left };

class RandomWalkAlgorithm : public MapGenerator
{
private:
	DirectionOfWalk walkDirection;
	sf::Vector2u spawnPoint;
	sf::Vector2u actualCoord;
	RandomNumberGenerator randomNumber;
	
	sf::Vector2u upDirection;
	sf::Vector2u rightDirection;
	sf::Vector2u downDirection;
	sf::Vector2u leftDirection;

	sf::Vector2u randomDirection;
	sf::Vector2u lastDirection;

	unsigned int maxTunnels;
	unsigned int maxLength;
	unsigned int actualLength;
public:
	RandomWalkAlgorithm(const sf::Vector2u tileAmount) : MapGenerator(tileAmount), walkDirection{}, maxTunnels{ 20 }, maxLength{}, upDirection{ sf::Vector2i{0, -1} }
		, rightDirection{ sf::Vector2i{1, 0} }, downDirection{ sf::Vector2i{0, 1} }, leftDirection{ sf::Vector2i{-1, 0} }, randomDirection{}, lastDirection{}
		, actualLength{}, actualCoord{} {}
	virtual void generateDungeonWithAlgorithm();

	void generateSpawnPointRandomly();
	void getWalkDirection();
	void getRandomMaxLength();
	void moveInDirection(sf::Vector2u& startingPoint, sf::Vector2u direction);
	bool checkBorderCollision();
	void buildDungeonTunnel();
};