#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include <TGUI/TGUI.hpp>
#include "ParticleManage.h"
#include "ParticlesInterface.h"

//#include "UI.h"

class windowSettings
{
private:
	sf::Vector2f m_activeWindowSize;
	sf::Vector2f m_GUIWindowSize;

	tgui::Gui m_gui;
	tgui::Slider::Ptr m_GravitySwitch;
	tgui::Slider::Ptr m_FrictionSwitch;
	tgui::Slider::Ptr m_AirResistanceSwitch;

	tgui::ListBox::Ptr m_Border;
	tgui::ListBox::Ptr m_ObjectType;
public:
	windowSettings(sf::RenderWindow& window, float border = 500.0f);
	const sf::Vector2f& getActiveWindowSize() { return m_activeWindowSize; }
	const sf::Vector2f& getGUIWindowSize() { return m_GUIWindowSize; }

	void transitionBorders(std::vector<std::unique_ptr<ParticlesInterface>>&);
	void erasingBorders(std::vector<std::unique_ptr<ParticlesInterface>>&);
	void reboundBorders(std::vector<std::unique_ptr<ParticlesInterface>>&);

	void loadGUI();
	void updateGUI(sf::Event& event) { m_gui.handleEvent(event); }
	void updateLogicGUI(windowSettings&, ParticleManage&);
	void drawGUI() { m_gui.draw(); }

	void colorParticlesByVelocity(ParticleManage&);

	auto& getObjectsUI() { return m_gui.getWidgets(); }
	auto getGravityButton() { return m_gui.get<tgui::Slider>("gravity"); }
	auto getGravityText() { return m_gui.get("tGravity"); }
	auto getFrictionButton() { return m_gui.get<tgui::Slider>("friction"); }
	auto getFrictionText() { return m_gui.get("tFriction"); }
	auto getAirResistanceButton() { return m_gui.get<tgui::Slider>("airResistance"); }
	auto getAirResistanceText() { return m_gui.get("tAirResistance"); }

	auto getBorders() { return m_gui.get<tgui::ListBox>("borderList"); }
	auto getObjectsType() { return m_gui.get<tgui::ListBox>("object list"); }
};
