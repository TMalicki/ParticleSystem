#include "ParticlesVertex.h"
#include <algorithm>

ParticlesVertex::ParticlesVertex(long int amount, sf::Vector2f position) : m_particleVertex(amount), ParticlesInterface(amount, position)
{
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex& particle) {particle.position = position; });
}

const std::vector<sf::Vector2f> ParticlesVertex::getPosition()
{
	std::vector<sf::Vector2f> tempPosition(m_particleVertex.size());
	size_t counter{};
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex particleVertex) {tempPosition.at(counter) = particleVertex.position; counter++; });
	
	return tempPosition;
}

void ParticlesVertex::setPosition(std::vector<sf::Vector2f> positions)
{
	size_t index{};
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex particleVertex) {particleVertex = positions.at(index); index++; });
}

void ParticlesVertex::eraseParticles(std::vector<size_t> index)
{
	/////////////////// TO JEST ŹLE chyba bo po wykonaniu erase dla pojedynczej iteracji indeksy się chyba zmieniają, więc jak np do usunięcia są elementy o indeksach 1,4,8,12,
	/////////////////// to na początku usunie element o indeksie 1, a później będzie chciał usunąć element o indeksie 4, przy czym wcześniej chyba nastąpi relokacja wektora?
	///////////////////	czy wtedy czasami element o indeksie 4 (początkowym indeksie), po usunięciu elementu wcześniejszego nie będzie na indexie 3? Bo cały wektor się zmniejszy.
	/////////////////// sprawdzić to!!!!!!!!!!!
	size_t indx{};
	//std::for_each(m_particleAttributes.begin(), m_particleAttributes.end(), [&]() {m_particleAttributes.erase(m_particleAttributes.begin() + index.at(indx)); indx++; });
	indx = 0;
	//std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&]() {m_particleVertex.erase(m_particleVertex.begin() + index.at(indx)); indx++; });
}

void ParticlesVertex::setColor(std::vector<sf::Color> colorVector)
{
	size_t index{};
	std::for_each(m_particleVertex.begin(), m_particleVertex.end(), [&](sf::Vertex particle) {particle.color = colorVector.at(index); index++; });
}

void ParticlesVertex::setDirectionTowardsPoint(sf::Vector2f goalPosition)
{
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		auto actualPosition = m_particleVertex[i].position;
		auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
		auto newDirectionMagnitude = sqrt(pow(newDirectionVector.x, 2) + pow(newDirectionVector.y, 2));
		if (newDirectionMagnitude == 0.0f) newDirectionMagnitude = 1.0f;

		auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };
		m_particleAttributes[i].setDirection(newDirection);
	}
}

void ParticlesVertex::update(float dt) // in parameter - dt
{
	//forceUpdate();
	for (size_t i = 0; i < m_particleVertex.size(); i++)
	{
		// is it needed to update both: position of each pixel and object of pixels like sf::Lines?
		auto tempPosition = m_particleVertex[i].position;
		auto maxVelocity = getMaxVelocity();

		//if (tempPosition.x >= 0 && tempPosition.x <= s_activeAreaSize.x && tempPosition.y >= 0 && tempPosition.y <= s_activeAreaSize.y)
		//{
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

			setParticleAttributesN(i, tempPosition, tempVelocity);
			//setParticleAttributes(i, tempPosition, tempVelocity, tempDirection);

			m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.0f, 0.0f });
	}
}


void ParticlesVertex::setParticleAttributesN(size_t index, sf::Vector2f position, sf::Vector2f velocity)
{
	m_particleVertex[index].position = position;
	m_particleAttributes[index].setVelocity(velocity);
}
