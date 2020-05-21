#include "ParticleSettings.h"

float ParticleSettings::m_maxLifeTimeMs = 5000.0f;

const sf::Vector2f ParticleSettings::getDirectionFromVelocity()
{
	auto newDirectionMagnitude = sqrt(pow(m_velocity.x, 2) + pow(m_velocity.y, 2));
	sf::Vector2f newDirection{};

	if (newDirectionMagnitude != 0.0f) newDirection = sf::Vector2f{ m_velocity.x / newDirectionMagnitude, m_velocity.y / newDirectionMagnitude };

	return newDirection;
}

void ParticleSettings::applyForce(sf::Vector2f force, Forces forceType, float constant)
{
	if(forceType == Forces::Gravity) 
	{
		m_direction = sf::Vector2f{ 0.0f, 1.0f };
		m_force = force;
	}
	else if (forceType == Forces::AirResistance)
	{
		float c = constant;
		auto velocityMagnitude = sqrt(pow(m_velocity.x, 2) + pow(m_velocity.y, 2));

		m_direction = -getDirectionFromVelocity();
		m_force = pow(velocityMagnitude, 2) * c * sf::Vector2f{1.0f, 1.0f};
	}
	else if (forceType == Forces::Friction)
	{
		auto normal = 1.0f;
		float mi = constant;

		m_direction = -getDirectionFromVelocity();
		m_force = normal * sf::Vector2f{ 1.0f * mi / 2.0f, 1.0f * mi };
	}
	else
	{
		m_force = (force / m_mass);
	}
	m_acceleration += sf::Vector2f{ abs(m_force.x) * m_direction.x, abs(m_force.y) * m_direction.y }; 
}