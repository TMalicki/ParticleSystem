#pragma once
#include <random>

class RandomNumberGenerator
{
private:
	std::random_device engineGenerator;
	std::mt19937 generator;
	std::uniform_int_distribution<int> integerDistribution;
public:
	RandomNumberGenerator()
		: engineGenerator{}, integerDistribution{}, generator{ engineGenerator() } {}

	int generateRandomInt(int min, int max) 
	{
		integerDistribution = std::uniform_int_distribution<int>(min, max);

		return integerDistribution(engineGenerator);
	}
};