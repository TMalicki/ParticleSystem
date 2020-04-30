#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include <TGUI/TGUI.hpp>
#include "ParticleManage.h"
#include "Particles.h"

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

//	UI m_UI_Objects;
public:
	windowSettings(sf::RenderWindow& window, float border = 500.0f);
	const auto getActiveWindowSize() { return m_activeWindowSize; }
	const auto getGUIWindowSize() { return m_GUIWindowSize; }

	//void transitionParticle(std::vector<sf::Vertex>&);
	void transitionBorders(std::vector<std::unique_ptr<Particles>>&);
	void erasingBorders(std::vector<std::unique_ptr<Particles>>&);
	void reboundBorders(std::vector<std::unique_ptr<Particles>>&);

	void loadGUI();
	void updateGUI(sf::Event& event) { m_gui.handleEvent(event); }
	//void updateLogicGUI(Particles&);
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

	auto getBorders() { return m_gui.get<tgui::ListBox>("Borders"); }
	//auto getToggleSwitch() { return m_gui.get("ToggleSwitch"); }
	//auto getTest() { return m_gui.get("Slider1"); }
	/*void createButton(sf::Vector2f size, sf::Vector2f position);
	void drawButton(sf::RenderWindow&);*/
};