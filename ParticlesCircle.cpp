#include <algorithm>
#include "ParticlesCircle.h"

ParticlesCircle::ParticlesCircle(long int amount, sf::Vector2f position) : m_particleCircle(amount), ParticlesInterface(amount, position)
{
	std::for_each(m_particleCircle.begin(), m_particleCircle.end(), 
		[&](sf::CircleShape& particle) 
		{
			auto x = 5.0f;
			particle.setRadius(x);
			particle.setOrigin(x, x); 
			particle.setPosition(position);
		});
}

const std::vector<sf::Vector2f> ParticlesCircle::getPosition()
{
	std::vector<sf::Vector2f> tempPosition(m_particleCircle.size());
	size_t counter{};
	std::for_each(m_particleCircle.begin(), m_particleCircle.end(), [&](sf::CircleShape& particleCircle) {tempPosition.at(counter) = particleCircle.getPosition(); counter++; });

	return tempPosition;
}

void ParticlesCircle::setPosition(std::vector<sf::Vector2f> positions)
{
	size_t index{};
	std::for_each(m_particleCircle.begin(), m_particleCircle.end(), [&](sf::CircleShape& particleCircle) {particleCircle.setPosition(positions.at(index)); index++; });
}

void ParticlesCircle::eraseParticles(std::vector<size_t> index)
{
	std::sort(index.begin(), index.end(), std::greater<size_t>());

	std::for_each(index.begin(), index.end(), [&](size_t& actualIndex) {m_particleAttributes[actualIndex] = m_particleAttributes.back(); m_particleAttributes.pop_back(); });
	std::for_each(index.begin(), index.end(), [&](size_t& actualIndex) {m_particleCircle[actualIndex] = m_particleCircle.back(); m_particleCircle.pop_back(); });
}

void ParticlesCircle::setColor(std::vector<sf::Color> colorVector)
{
	size_t index{};
	std::for_each(m_particleCircle.begin(), m_particleCircle.end(), [&](sf::CircleShape& particle) {particle.setFillColor(colorVector.at(index)); index++; });
}

void ParticlesCircle::setDirectionTowardsPoint(sf::Vector2f goalPosition)
{
	for (size_t i = 0; i < m_particleCircle.size(); i++)
	{
		auto actualPosition = m_particleCircle[i].getPosition();
		auto newDirectionVector = sf::Vector2f{ goalPosition.x - actualPosition.x, goalPosition.y - actualPosition.y };
		auto newDirectionMagnitude = sqrt(pow(newDirectionVector.x, 2) + pow(newDirectionVector.y, 2));
		if (newDirectionMagnitude == 0.0f) newDirectionMagnitude = 1.0f;

		auto newDirection = sf::Vector2f{ newDirectionVector.x / newDirectionMagnitude, newDirectionVector.y / newDirectionMagnitude };
		m_particleAttributes[i].setDirection(newDirection);
	}
}

void ParticlesCircle::update(float dt) // in parameter - dt
{
	///// this should be added to emiterEffect class?
	reduceLifeTime(dt);
	auto cutOff = std::lower_bound(m_particleAttributes.begin(), m_particleAttributes.end(), 0.0f, [&](ParticleSettings attributes, const float b) { return attributes.getLifeTime() < b; });

	m_particleCircle.erase(m_particleCircle.begin(), m_particleCircle.begin() + std::distance(m_particleAttributes.begin(), cutOff));
	m_particleAttributes.erase(m_particleAttributes.begin(), cutOff);
	/////

	//forceUpdate();
	for (size_t i = 0; i < m_particleCircle.size(); i++)
	{
		// is it needed to update both: position of each pixel and object of pixels like sf::Lines?
		auto tempPosition = m_particleCircle[i].getPosition();
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

		setParticleAttributes(i, tempPosition, tempVelocity);
		m_particleAttributes[i].setAcceleration(sf::Vector2f{ 0.0f, 0.0f });
	}
	//}
	//else
	//{
		//eraseParticle
	//}
}

void ParticlesCircle::setParticleAttributes(size_t index, sf::Vector2f position, sf::Vector2f velocity)
{
	m_particleCircle[index].setPosition(position);
	m_particleAttributes[index].setVelocity(velocity);
}

void ParticlesCircle::draw(sf::RenderWindow& window)
{
	for (auto& particleCircle : m_particleCircle)
	{
		window.draw(particleCircle);
	}
}
