#include "windowSettings.h"

windowSettings::windowSettings(sf::RenderWindow& window, float border) : m_gui{window}
{
	auto windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
	m_activeWindowSize = sf::Vector2f{ windowSize.x - border, windowSize.y };
	m_GUIWindowSize = sf::Vector2f{ border, windowSize.y };
}

void windowSettings::transitionParticle(std::vector<sf::Vertex>& particlesVertex)
{
	for (size_t i = 0; i < particlesVertex.size(); i++)
	{
		auto tempPosition = particlesVertex[i].position;

		if (tempPosition.y < 0) particlesVertex[i].position.y = m_activeWindowSize.y;
		else if (tempPosition.y > m_activeWindowSize.y) particlesVertex[i].position.y = 0;
		else if (tempPosition.x < 0) particlesVertex[i].position.x = m_activeWindowSize.x;
		else if (tempPosition.x > m_activeWindowSize.x) particlesVertex[i].position.x = 0;
	}
}

