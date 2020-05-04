#include "windowSettings.h"


windowSettings::windowSettings(sf::RenderWindow& window, float border) : m_gui{window}
{
	auto windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
	m_activeWindowSize = sf::Vector2f{ windowSize.x - border, windowSize.y };
	m_GUIWindowSize = sf::Vector2f{ border, windowSize.y };
}

void windowSettings::transitionBorders(std::vector<std::unique_ptr<ParticlesInterface>>& particles)
{
	for (auto& particle : particles)
	{
		auto particlesPositions = particle->getPosition();
		std::vector<sf::Vector2f> tempPositions(particlesPositions.size());

		for (size_t i = 0; i < particlesPositions.size(); i++)
		{
			if (particlesPositions.at(i).y < 0) tempPositions.at(i) = sf::Vector2f{ particlesPositions.at(i).x, m_activeWindowSize.y };
			else if (particlesPositions.at(i).y > m_activeWindowSize.y) tempPositions.at(i) = sf::Vector2f{ particlesPositions.at(i).y, 0.0f };
			else if (particlesPositions.at(i).x < 0) tempPositions.at(i) = sf::Vector2f{ m_activeWindowSize.x, particlesPositions.at(i).y }; 
			else if (particlesPositions.at(i).x > m_activeWindowSize.x) tempPositions.at(i) = sf::Vector2f{ 0.0f, particlesPositions.at(i).y };
		}
		particle->setPosition(tempPositions);
	}
}

void windowSettings::erasingBorders(std::vector<std::unique_ptr<ParticlesInterface>>& particles)
{
	std::vector<size_t> toEraseGroup{};
	size_t counter{ 0 };

	for (auto& particle : particles)
	{
		//auto& particlesVertex = particle->getParticleVertex();
		//auto& particlesAttributes = particle->getParticleAttributes();
		auto particlesPositions = particle->getPosition();
		std::vector<sf::Vector2f> tempPositions(particlesPositions.size());

		std::vector<size_t> toErase{};

		for (size_t i = 0; i < particlesPositions.size(); i++)
		{
			auto tempPosition = particlesPositions[i];

			if (tempPosition.y <= 0 || tempPosition.y >= m_activeWindowSize.y) toErase.push_back(i);
			else if (tempPosition.x <= 0 || tempPosition.x >= m_activeWindowSize.x) toErase.push_back(i);
		}

		particle->eraseParticles(toErase);
		if (particle->getParticlesAmount() == 0) toEraseGroup.push_back(counter);
		
		counter++;
	}
	for (auto erase : toEraseGroup)
	{
		particles.erase(particles.begin() + erase);
	}
}

void windowSettings::reboundBorders(std::vector<std::unique_ptr<ParticlesInterface>>& particles)
{
	for (auto& particle : particles)
	{
		//auto& particlesVertex = particle->getParticleVertex();
		auto& particlesAttributes = particle->getParticleAttributes();

		size_t size = particle->getParticlesAmount();
		auto particlesPositions = particle->getPosition();
		auto particlesVelocities = particle->getVelocity();

		std::vector<sf::Vector2f> tempPositions(particlesPositions.size());
		std::vector<sf::Vector2f> tempVelocities(particlesVelocities.size());
		/*
		for (size_t i = 0; i < size; i++)
		{
			if (particlesPositions.at(i).y <= 0 || particlesPositions.at(i).y >= m_activeWindowSize.y) tempVelocities.at(i) = sf::Vector2f{ particlesVelocities.at(i).x, -particlesVelocities.at(i).y };
			else if (particlesPositions.at(i).x <= 0 || particlesPositions.at(i).x >= m_activeWindowSize.x) tempVelocities.at(i) = sf::Vector2f{ -particlesVelocities.at(i).x, particlesVelocities.at(i).y };
		}
		*/
		size_t index{};
		std::for_each(particlesPositions.begin(), particlesPositions.end(), 
		[&](sf::Vector2f tempPositions)
		{
				if (tempPositions.y <= 0 || tempPositions.y >= m_activeWindowSize.y)
				{
					tempVelocities.at(index) = sf::Vector2f{ particlesVelocities.at(index).x, -particlesVelocities.at(index).y }; 
				}
				else if (tempPositions.x <= 0 || tempPositions.x >= m_activeWindowSize.x)
				{
					tempVelocities.at(index) = sf::Vector2f{ -particlesVelocities.at(index).x, particlesVelocities.at(index).y };
				}
				index++;
			});

		particle->setVelocity(tempVelocities);
	}
}

void windowSettings::colorParticlesByVelocity(ParticleManage& particles)
{
	if (particles.getExplodedParticles().size() > 0)
	{
		auto maxVelocity = particles.getExplodedParticles()[0]->getMaxVelocity();

		for (auto& particle : particles.getExplodedParticles())
		{
			size_t size = particle->getParticlesAmount();

			auto tempVelocities = particle->getVelocity();
			std::vector<sf::Color> tempColor(tempVelocities.size());

			for (size_t i = 0; i < size; i++)
			{
				auto modVelocity = sqrt(pow(tempVelocities.at(i).x, 2) + pow(tempVelocities.at(i).y, 2));
				int calculatedRGB = 255 - static_cast<int>(modVelocity / maxVelocity * 255.0f);

				tempColor.at(i) = sf::Color(255, calculatedRGB, calculatedRGB, 255);
			}
			particle->setColor(tempColor);
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


