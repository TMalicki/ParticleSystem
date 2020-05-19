#include "windowSettings.h"

windowSettings::windowSettings(sf::RenderWindow& window, float border) : m_gui{ window }, m_type{ ParticleType::Vertex }
{
	auto windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
	m_activeWindowSize = sf::Vector2f{ windowSize.x - border, windowSize.y };
	m_GUIWindowSize = sf::Vector2f{ border, windowSize.y };
	m_AirResistanceOn = false;
	m_FrictionOn = false;
	m_GravityOn = false;
	m_WindOn = false;
	m_LifeTimeOn = false;
}

void windowSettings::loadGUI()
{
	m_gui.loadWidgetsFromFile("GUI/SideGUI.txt");
	m_GravitySwitch = getGravityButton();
	m_FrictionSwitch = getFrictionButton();
	m_AirResistanceSwitch = getAirResistanceButton();
	m_WindSwitch = getWindSwitch();
	m_WindDirection = getWindDirectionKnob();

	m_EffectText = getEffectType();
	m_LifeTime = getLifeTime();

	m_Border = getBorders();
	m_ObjectType = getObjectsType();
}

std::vector<sf::Vector2f> windowSettings::transitionBorders(const std::vector<sf::Vector2f>& positions)
{
	std::vector<sf::Vector2f> tempPositions = positions;
	size_t size{};

	std::for_each(positions.begin(), positions.end(), [&](auto& position)
	{
		if (position.y < 0) tempPositions[size] = sf::Vector2f{ position.x, m_activeWindowSize.y };
		else if (position.y > m_activeWindowSize.y) tempPositions[size] = sf::Vector2f{ position.x, 0.0f };
		else if (position.x < 0) tempPositions[size] = sf::Vector2f{ m_activeWindowSize.x, position.y };
		else if (position.x > m_activeWindowSize.x) tempPositions[size] = sf::Vector2f{ 0.0f, position.y };
		size++;
	});
	return tempPositions;
}

std::vector<size_t> windowSettings::erasingBorders(const std::vector<sf::Vector2f>& positions)
{
	std::vector<sf::Vector2f> tempPositions = positions;
	std::vector<size_t> toErase{};
	size_t index{};

	std::for_each(positions.begin(), positions.end(), [&](auto& position)
	{
		if (position.y <= 0 || position.y >= m_activeWindowSize.y) toErase.push_back(index);
		else if (position.x <= 0 || position.x >= m_activeWindowSize.x) toErase.push_back(index);
		index++;
	});
	return toErase;
}

std::vector<sf::Vector2f> windowSettings::reboundBorders(const std::vector<sf::Vector2f>& positions, const std::vector<sf::Vector2f>& velocities)
{
	std::vector<sf::Vector2f> tempPositions = positions;
	std::vector<sf::Vector2f> tempVelocities = velocities;
	size_t index{};

	std::for_each(positions.begin(), positions.end(), [&](auto& position)
	{
		if (position.y <= 0 || position.y >= m_activeWindowSize.y)
		{
			tempVelocities[index] = sf::Vector2f{ tempVelocities[index].x, -tempVelocities[index].y };
		}
		else if (position.x <= 0 || position.x >= m_activeWindowSize.x)
		{
			tempVelocities[index] = sf::Vector2f{ -tempVelocities[index].x, tempVelocities[index].y };
		}
		index++;
	});
	return tempVelocities;
}

windowSettings::BorderType windowSettings::getBorderType()
{
	if (m_Border->getSelectedItem() == "Rebound Border")
	{
		return BorderType::ReboundBorder;
	}
	else if (m_Border->getSelectedItem() == "Erasing Border")
	{
		return BorderType::ErasingBorder;
	}
	else if (m_Border->getSelectedItem() == "Transition Border")
	{
		return BorderType::TransitionBorder;
	}
}

void windowSettings::updateLogicGUI()
{
	if (m_GravitySwitch->getValue() == 0.0f) m_GravitySwitch->connect("ValueChanged", [&]() { m_GravityOn = true; });
	else m_GravitySwitch->connect("ValueChanged", [&]() { m_GravityOn = false; });

	if (m_FrictionSwitch->getValue() == 0.0f) m_FrictionSwitch->connect("ValueChanged", [&]() { m_FrictionOn = true; });
	else m_FrictionSwitch->connect("ValueChanged", [&]() { m_FrictionOn = false; });

	if (m_AirResistanceSwitch->getValue() == 0.0f) m_AirResistanceSwitch->connect("ValueChanged", [&]() { m_AirResistanceOn = true; });
	else m_AirResistanceSwitch->connect("ValueChanged", [&]() { m_AirResistanceOn = false; });

	if (m_WindSwitch->getValue() == 0.0f) m_WindSwitch->connect("ValueChanged", [&]() { m_WindOn = true; });
	else m_WindSwitch->connect("ValueChanged", [&]() { m_WindOn = false; });

	if (m_ObjectType->getSelectedItem() == "Vertex") { m_type = ParticleType::Vertex; } 
	else if (m_ObjectType->getSelectedItem() == "Circle") { m_type = ParticleType::CircleShape; };  

	m_LifeTime->connect("Checked", [&]() { m_LifeTimeOn = true; });
	m_LifeTime->connect("unchecked", [&]() { m_LifeTimeOn = false; });
}



