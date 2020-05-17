#include "EmiterEffect.h"

void EmiterEffect::createEmiter(sf::Vector2i mousePosition)
{
	//m_EmiterOn = true;
	m_EmiterObject.push_back(sf::CircleShape(4.0f, 20));
	m_EmiterObject.back().setOutlineColor(sf::Color::White);
	m_EmiterObject.back().setOrigin(4.0f, 4.0f);
	m_EmiterObject.back().setPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	m_EmiterCounter.push_back(size_t(0));
	m_EmiterTimer.push_back(0.0f);
}

void EmiterEffect::updateEmiter(float dt)
{
	for (size_t i = 0; i < m_EmiterObject.size(); i++)
	{
		//auto mousePosition = m_EmiterObject[i].getPosition();
		if (m_EmiterTimer[i] >= 200.0f)
		{
			m_EmiterOn = true;
			m_EmiterCounter[i] += 1;
			m_EmiterTimer[i] = 0.0f;
		}
		if (m_EmiterCounter[i] >= 10)
		{
			m_EmiterCounter[i] = 0;
			m_EmiterOn = false;
			m_EmiterObject[i] = m_EmiterObject.back();
			m_EmiterObject.pop_back();
		}
		m_EmiterTimer[i] += dt;
	}

}

const std::vector<sf::Vector2f> EmiterEffect::getEmitersPositions()
{
	std::vector<sf::Vector2f> mousePositions(m_EmiterObject.size());

	for (size_t i = 0; i < mousePositions.size(); i++)
	{
		mousePositions[i] = m_EmiterObject[i].getPosition();
	}
	// TODO: insert return statement here
	return mousePositions;
}

void EmiterEffect::draw(sf::RenderWindow& window)
{
	for (auto& emiter : m_EmiterObject)
	{
		window.draw(emiter);
	}
}
