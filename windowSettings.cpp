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

void windowSettings::loadGUI()
{ 
	m_gui.loadWidgetsFromFile("GUI/SideGUI.txt"); 
	m_GravitySwitch = getGravityButton();
	m_FrictionSwitch = getFrictionButton();
	m_AirResistanceSwitch = getAirResistanceButton();
}

void windowSettings::updateLogicGUI(Particles& particles)
{	
	if (m_GravitySwitch->getValue() == false) m_GravitySwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::Gravity); });
	else m_GravitySwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::Gravity); });

	if (m_FrictionSwitch->getValue() == false) m_FrictionSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::Friction); });
	else m_FrictionSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::Friction); });

	if (m_AirResistanceSwitch->getValue() == false) m_AirResistanceSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::AirResistance); });
	else m_AirResistanceSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::AirResistance); });
}

