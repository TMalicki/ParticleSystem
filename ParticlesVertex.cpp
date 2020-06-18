#include "ParticlesVertex.h"
#include <algorithm>

ParticlesVertex::ParticlesVertex(long int amount, sf::Vector2f position) : m_particleVertex(amount), ParticlesInterface(amount, position)
{
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex& particle) {particle.position = position; });
}

// here should be something like downloadPosition(vector<sf::Vector2f&> because here i have refence of local variable so copy is needed. I do not want that
const std::vector<sf::Vector2f> ParticlesVertex::getPosition()
{
	std::vector<sf::Vector2f> tempPosition(m_particleVertex.size());
	size_t counter{};
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex& particleVertex) {tempPosition[counter] = particleVertex.position; counter++; });
	
	return tempPosition;
}

void ParticlesVertex::setPosition(std::vector<sf::Vector2f> positions)
{
	size_t index{};
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex& particleVertex) {particleVertex = positions[index]; index++; });
}

void ParticlesVertex::eraseParticles(std::vector<size_t> index)
{
	std::sort(index.begin(), index.end(), std::greater<size_t>());

	std::for_each(index.begin(), index.end(), [&](size_t& actualIndex) {m_particleAttributes[actualIndex] = m_particleAttributes.back(); m_particleAttributes.pop_back(); });
	std::for_each(index.begin(), index.end(), [&](size_t& actualIndex) {m_particleVertex[actualIndex] = m_particleVertex.back(); m_particleVertex.pop_back(); });
}

void ParticlesVertex::eraseParticles(std::vector<ParticleSettings>::iterator cutOff)
{
	m_particleVertex.erase(m_particleVertex.begin(), m_particleVertex.begin() + std::distance(m_particleAttributes.begin(), cutOff));
	m_particleAttributes.erase(m_particleAttributes.begin(), cutOff);
}

std::vector<sf::Color> ParticlesVertex::getColor()
{
	std::vector<sf::Color> tempColors(m_particleVertex.size());
	size_t index{};
	for (auto& particle : m_particleVertex)
	{
		tempColors[index] = particle.color;
		index++;
	}
	return tempColors;
}

void ParticlesVertex::setColor(std::vector<sf::Color> colorVector) // use reference?
{
	size_t index{};
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex& particle) {particle.color = colorVector.at(index); index++; });
}

void ParticlesVertex::reduceColorOpacity(int value)
{
	for (auto& particle : m_particleVertex)
	{
		if (particle.color.a > 0) particle.color.a -= value;
		else particle.color.a = 0;
	}
}

void ParticlesVertex::setDirectionTowardsPoint(sf::Vector2f goalPosition) //?
{
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		auto& actualPosition = m_particleVertex[i].position;
		auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
		auto newDirectionMagnitude = sqrt(pow(newDirectionVector.x, 2) + pow(newDirectionVector.y, 2));
		if (newDirectionMagnitude == 0.0f) newDirectionMagnitude = 1.0f;

		auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };
		m_particleAttributes[i].setDirection(newDirection);
	}
}

void ParticlesVertex::update(float dt) //?
{
	//forceUpdate();
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		auto tempPosition = m_particleVertex[i].position;
		auto maxVelocity = getMaxVelocity();

		sf::Vector2f tempDirection = m_particleAttributes[i].getDirection();
		sf::Vector2f tempVelocity = m_particleAttributes[i].getVelocity();
		sf::Vector2f tempAcceleration = m_particleAttributes[i].getAcceleration();

		tempVelocity.x += tempAcceleration.x; 
		tempVelocity.y += tempAcceleration.y; 

		float magnitudeVector = sqrt(pow(tempVelocity.x, 2) + pow(tempVelocity.y, 2));
		if (magnitudeVector > maxVelocity)
		{
			tempVelocity = tempVelocity / magnitudeVector * maxVelocity;
		}

		tempPosition += tempVelocity * dt / 100.0f;

		// make setParticleAttributes for vector maybe?
		setParticleAttributes(i, tempPosition, tempVelocity);
		m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.0f, 0.0f });
	}
}

void ParticlesVertex::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity) // is it used?
{
	m_particleVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
}
