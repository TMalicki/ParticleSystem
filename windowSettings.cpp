#include "windowSettings.h"

windowSettings::windowSettings(sf::RenderWindow& window, float border) : m_gui{window}
{
	auto windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
	m_activeWindowSize = sf::Vector2f{ windowSize.x - border, windowSize.y };
	m_GUIWindowSize = sf::Vector2f{ border, windowSize.y };
}

void windowSettings::transitionBorders(std::vector<std::unique_ptr<Particles>>& particles)
{
	for (auto& particle : particles)
	{
		auto& particlesVertex = particle->getParticleVertex();
	
		for (size_t i = 0; i < particlesVertex.size(); i++)
		{
			auto tempPosition = particlesVertex[i].position;

			if (tempPosition.y < 0) particlesVertex[i].position.y = m_activeWindowSize.y;
			else if (tempPosition.y > m_activeWindowSize.y) particlesVertex[i].position.y = 0;
			else if (tempPosition.x < 0) particlesVertex[i].position.x = m_activeWindowSize.x;
			else if (tempPosition.x > m_activeWindowSize.x) particlesVertex[i].position.x = 0;
		}
	}
}

void windowSettings::erasingBorders(std::vector<std::unique_ptr<Particles>>& particles)
{
	std::vector<size_t> toEraseGroup{};
	size_t counter{ 0 };

	for (auto& particle : particles)
	{
		auto& particlesVertex = particle->getParticleVertex();
		auto& particlesAttributes = particle->getParticleAttributes();

		std::vector<size_t> toErase{};

		for (size_t i = 0; i < particlesVertex.size(); i++)
		{
			auto tempPosition = particlesVertex[i].position;

			if (tempPosition.y <= 0 || tempPosition.y >= m_activeWindowSize.y) toErase.push_back(i);
			else if (tempPosition.x < 0 || tempPosition.x > m_activeWindowSize.x) toErase.push_back(i);
		}

		for (auto erase : toErase)
		{
			particlesVertex.erase(particlesVertex.begin() + erase);
			particlesAttributes.erase(particlesAttributes.begin() + erase);
		}
		if (particlesVertex.size() == 0) toEraseGroup.push_back(counter);
		counter++;
	}
	for (auto erase : toEraseGroup)
	{
		particles.erase(particles.begin() + erase);
	}
}

void windowSettings::reboundBorders(std::vector<std::unique_ptr<Particles>>& particles)
{
	for (auto& particle : particles)
	{
		auto& particlesVertex = particle->getParticleVertex();
		auto& particlesAttributes = particle->getParticleAttributes();

		for (size_t i = 0; i < particlesVertex.size(); i++)
		{
			auto tempPosition = particlesVertex[i].position;
			auto tempVelocity = particlesAttributes.at(i).getVelocity();

			if (tempPosition.y <= 0 /*&& tempVelocity.y < 0*/ || tempPosition.y >= m_activeWindowSize.y /*&& tempVelocity.y < 0*/) particlesAttributes.at(i).setVelocity(sf::Vector2f{ tempVelocity.x, -tempVelocity.y });
			else if (tempPosition.x < 0 /*&& tempVelocity.x < 0*/ || tempPosition.x > m_activeWindowSize.x /*&& tempVelocity.x < 0*/) particlesAttributes.at(i).setVelocity(sf::Vector2f{ -tempVelocity.x, tempVelocity.y });
		}
	}
}

void windowSettings::loadGUI()
{ 
	m_gui.loadWidgetsFromFile("GUI/SideGUI.txt"); 
	m_GravitySwitch = getGravityButton();
	m_FrictionSwitch = getFrictionButton();
	m_AirResistanceSwitch = getAirResistanceButton();

	m_Border = getBorders();
}

void windowSettings::updateLogicGUI(windowSettings& windowSettings, ParticleManage& particles)
{
	if (m_GravitySwitch->getValue() == 0.0f) m_GravitySwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::Gravity); });
	else m_GravitySwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::Gravity); });

	if (m_FrictionSwitch->getValue() == 0.0f) m_FrictionSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::Friction); });
	else m_FrictionSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::Friction); });

	if (m_AirResistanceSwitch->getValue() == 0.0f) m_AirResistanceSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::AirResistance); });
	else m_AirResistanceSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::AirResistance); });

	if (m_Border->getSelectedItem() == "Erasing Border") windowSettings.erasingBorders(particles.getExplodedParticles());
	else if (m_Border->getSelectedItem() == "Rebound Border") windowSettings.reboundBorders(particles.getExplodedParticles());
	else if (m_Border->getSelectedItem() == "Transition Border") windowSettings.transitionBorders(particles.getExplodedParticles());
}

void windowSettings::colorParticlesByVelocity(ParticleManage& particles)
{
	if (particles.getExplodedParticles().size() > 0)
	{
		auto maxVelocity = particles.getExplodedParticles()[0]->getMaxVelocity();

		for (auto& particle : particles.getExplodedParticles())
		{
			for (size_t i = 0; i < particle->getParticleVertex().size(); i++)
			{
				auto velocity = particle->getParticleAttributes()[i].getVelocity();
				auto modVelocity = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));

				int tempColor = 255 - static_cast<int>(modVelocity / maxVelocity * 255.0f);

				particle->getParticleVertex()[i].color = sf::Color(255, tempColor, tempColor, 255);
			}
		}
	}
}

