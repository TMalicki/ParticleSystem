#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include <TGUI/TGUI.hpp>

//#include "UI.h"

class windowSettings
{
private:
	sf::Vector2f m_activeWindowSize;
	sf::Vector2f m_GUIWindowSize;

	tgui::Gui m_gui;
//	UI m_UI_Objects;
public:
	windowSettings(sf::RenderWindow& window, float border = 500.0f);
	const auto getActiveWindowSize() { return m_activeWindowSize; }
	const auto getGUIWindowSize() { return m_GUIWindowSize; }

	void transitionParticle(std::vector<sf::Vertex>&);
	//void loadGUI(tgui::Gui& gui) { gui.loadWidgetsFromFile("GUI/Side.txt"); }


	void loadGUI() { m_gui.loadWidgetsFromFile("GUI/Side.txt"); }
	void updateGUI(sf::Event& event) { m_gui.handleEvent(event); }
	void drawGUI() { m_gui.draw(); }

	auto& getObjectsUI() { return m_gui.getWidgets(); }




	/*void createButton(sf::Vector2f size, sf::Vector2f position);
	void drawButton(sf::RenderWindow&);*/
};