#include "windowSettings.h"

windowSettings::windowSettings(sf::RenderWindow& window, float border) : m_gui{window}
{
	auto windowSize = sf::Vector2f{ static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y) };
	m_activeWindowSize = sf::Vector2f{ windowSize.x - border, windowSize.y };
	m_GUIWindowSize = sf::Vector2f{ border, windowSize.y };
}

void windowSettings::loadGUI()
{
	m_gui.loadWidgetsFromFile("GUI/SideGUI.txt");
	m_GravitySwitch = getGravityButton();
	m_FrictionSwitch = getFrictionButton();
	m_AirResistanceSwitch = getAirResistanceButton();
	m_WindSwitch = getWindSwitch();
	m_WindDirection = getWindDirection();

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

// change for better encapsulation
void windowSettings::colorParticlesByVelocity(ParticleManage& particles)
{
	if (particles.getExplodedParticles().size() > 0)
	{
		auto maxVelocity = particles.getExplodedParticles()[0]->getMaxVelocity();

		for (auto& particle : particles.getExplodedParticles())
		{
			size_t size = particle->getParticlesAmount();

			auto tempVelocities = particle->getVelocity();
			auto tempColor = particle->getColor();
		
			for (size_t i = 0; i < size; i++)
			{
				auto modVelocity = sqrt(pow(tempVelocities.at(i).x, 2) + pow(tempVelocities.at(i).y, 2));
				int calculatedRGB = 255 - static_cast<int>((modVelocity / maxVelocity) * 380.0f);	///380, not 255 for faster red color achieved
				if (calculatedRGB >= 255) calculatedRGB = 255;

				tempColor.at(i) = sf::Color(255, calculatedRGB, calculatedRGB, tempColor[i].a);
			}
			particle->setColor(tempColor); // maybe this?
		}
	}
	if (particles.getEmiterParticles().size() > 0)
	{
		auto maxVelocity = particles.getEmiterParticles()[0]->getMaxVelocity();

		for (auto& particle : particles.getEmiterParticles())
		{
			size_t size = particle->getParticlesAmount();

			auto tempVelocities = particle->getVelocity();
			auto tempColor = particle->getColor();

			for (size_t i = 0; i < size; i++)
			{
				auto modVelocity = sqrt(pow(tempVelocities.at(i).x, 2) + pow(tempVelocities.at(i).y, 2));
				int calculatedRGB = 255 - static_cast<int>((modVelocity / maxVelocity) * 380.0f);	///380, not 255 for faster red color achieved
				if (calculatedRGB >= 255) calculatedRGB = 255;

				tempColor.at(i) = sf::Color(255, calculatedRGB, calculatedRGB, tempColor[i].a);
			}
			particle->setColor(tempColor); // maybe this?
		}
	}
}

// change for better encapsulation
void windowSettings::updateLogicGUI(windowSettings& windowSettings, ParticleManage& particles)
{
	if (m_GravitySwitch->getValue() == 0.0f) m_GravitySwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::Gravity); });
	else m_GravitySwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::Gravity); });

	if (m_FrictionSwitch->getValue() == 0.0f) m_FrictionSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::Friction); });
	else m_FrictionSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::Friction); });

	if (m_AirResistanceSwitch->getValue() == 0.0f) m_AirResistanceSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::AirResistance); });
	else m_AirResistanceSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::AirResistance); });

	if (m_WindSwitch->getValue() == 0.0f) m_WindSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(true, ParticleSettings::Forces::External); });
	else m_WindSwitch->connect("ValueChanged", [&]() { particles.TurnOnForce(false, ParticleSettings::Forces::External); });

	if (m_ObjectType->getSelectedItem() == "Vertex") { particles.setParticleType(ParticleManage::ParticleType::Vertex); } // particleVertex
	else if (m_ObjectType->getSelectedItem() == "Circle") { particles.setParticleType(ParticleManage::ParticleType::CircleShape); };  // circleShape

	m_LifeTime->connect("Checked", [&]() { particles.applyFading(true); });
	m_LifeTime->connect("unchecked", [&]() { particles.applyFading(false); });

	if (m_WindDirection->connect("ValueChanged", [&]()
	{
		float temp = static_cast<float>(m_WindDirection->getValue());
		particles.setWindDirection(sf::Vector2f{ -sin(temp * 3.14f / 180.0f), cos(temp * 3.14f / 180.0f) });
	}
	)) {}
	///////
	if (m_Border->getSelectedItem() == "Rebound Border")
	{
		auto& temp = particles.getExplodedParticles();
		for (size_t i = 0; i < temp.size(); i++)
		{
			temp[i]->setVelocity(windowSettings.reboundBorders(temp[i]->getPosition(), temp[i]->getVelocity()));
		}

		auto& temp1 = particles.getEmiterParticles();
		for (size_t i = 0; i < temp1.size(); i++)
		{
			temp1[i]->setVelocity(windowSettings.reboundBorders(temp1[i]->getPosition(), temp1[i]->getVelocity()));
		}
	}
	else if (m_Border->getSelectedItem() == "Erasing Border")
	{
		auto& temp = particles.getExplodedParticles();
		std::vector<size_t> toEraseGroup{};

		for (size_t i = 0; i < temp.size(); i++)
		{
			temp[i]->eraseParticles(windowSettings.erasingBorders(temp[i]->getPosition()));
			if (temp[i]->getParticlesAmount() == 0)
			{
				toEraseGroup.push_back(i);
			}
		}

		std::sort(toEraseGroup.begin(), toEraseGroup.end(), std::greater<size_t>());
		for (auto erase : toEraseGroup)
		{
			*temp.at(erase) = *temp.back();
			temp.pop_back();
		}

		auto& temp1 = particles.getEmiterParticles();
		std::vector<size_t> toEraseGroup1{};

		for (size_t i = 0; i < temp1.size(); i++)
		{
			temp1[i]->eraseParticles(windowSettings.erasingBorders(temp1[i]->getPosition()));
			if (temp1[i]->getParticlesAmount() == 0)
			{
				toEraseGroup1.push_back(i);
			}
		}

		std::sort(toEraseGroup1.begin(), toEraseGroup1.end(), std::greater<size_t>());
		for (auto erase : toEraseGroup1)
		{
			*temp1.at(erase) = *temp1.back();
			temp1.pop_back();
		}
	}
	else if (m_Border->getSelectedItem() == "Transition Border") 
	{
		auto& temp = particles.getExplodedParticles();
		for (size_t i = 0; i < temp.size(); i++)
		{
			temp[i]->setPosition(windowSettings.transitionBorders(temp[i]->getPosition()));
		}

		auto& temp1 = particles.getEmiterParticles();
		for (size_t i = 0; i < temp1.size(); i++)
		{
			temp1[i]->setPosition(windowSettings.transitionBorders(temp1[i]->getPosition()));
		}
	}
}


