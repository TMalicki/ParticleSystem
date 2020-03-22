#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int amount) : particles(sf::Points, amount)
{
	srand(time(NULL));
	std::cout << "Utworzono ParticleSystem\n";
	for (int i = 0; i < amount; i++)
	{
		particles[i].position = sf::Vector2f(800,500);
		particles[i].color = sf::Color::Red;
	}
}

ParticleSystem::ParticleSystem(const ParticleSystem& copyParticles)
{
	particles = copyParticles.particles;
	std::cout << "Skopiowano\n";
}

ParticleSystem& ParticleSystem::operator=(const ParticleSystem& assignParticles)
{
	if (this != &assignParticles)
	{
		particles = assignParticles.particles;
		std::cout << "przepisano\n";
	}

	return *this;
}

void ParticleSystem::move(sf::Vector2i mousePosition)
{
	int randomPositionX;
	int randomPositionY;
	sf::Vector2f actualPosition;
	int valueOfColor;

	for (int i = 0; i < particles.getVertexCount(); i++)
	{
		actualPosition = particles[i].position;
		randomPositionX = (rand() % 100 - 50) / 2;
		randomPositionY = (rand() % 100 - 50) / 2;
		sf::Vector2f diffPosition((mousePosition.x - actualPosition.x)/50 + randomPositionX, (mousePosition.y - actualPosition.y)/50 + randomPositionY);
		particles[i].position += diffPosition;

		//particles[i].position += sf::Vector2f(randomPositionX, randomPositionY);
		
		valueOfColor = (sqrt(pow(mousePosition.x - actualPosition.x, 2) + pow(mousePosition.y - actualPosition.y, 2))) * 255 / 1900;
		//std::cout << valueOfColor << std::endl;

		particles[i].color = sf::Color(0, 255 - valueOfColor, 0, 255);
	}
}

void ParticleSystem::explode(sf::Vector2i mousePosition)
{
	sf::Vector2f actualPosition;
	sf::Vector2f actualVelocity;
	int randomPositionX;
	int randomPositionY;

	for (int i = 0; i < particles.getVertexCount(); i++)
	{
		randomPositionX = (rand() % 2 - 1);
		randomPositionY = (rand() % 2 - 1);
		actualPosition = particles[i].position;
		actualVelocity = sf::Vector2f(((mousePosition.x - actualPosition.x) / 50) + randomPositionX, ((mousePosition.y - actualPosition.y) / 50) + randomPositionY);

		particles[i].position += actualVelocity;
	}
}

