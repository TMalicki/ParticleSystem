#pragma once
#include <iostream>
#include <TGUI/TGUI.hpp>
#include "windowSettings.h"

#include "ParticleManage.h"
//#include "Timer.h"

class ParticleSystem
{
private:
	sf::Vector2f windowSize;
	float settingWindowWidth;

	sf::RenderWindow window;
	std::unique_ptr<windowSettings> windowSetting;
	std::unique_ptr<ParticleManage> m_particlesManage;

public:
	ParticleSystem(sf::Vector2f windowSize = sf::Vector2f{ 1920.0f, 1080.0f });
	void Run();


	void applyForces();
	void erasingBorder(ParticleManage::ParticleEffect);
	void reboundBorder(ParticleManage::ParticleEffect);
	void transitionBorder(ParticleManage::ParticleEffect);
	void applyBorders();
};

