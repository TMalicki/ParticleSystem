#pragma once
#include <iostream>
#include <TGUI/TGUI.hpp>

class windowSettings
{
public:
	enum class BorderType { ReboundBorder, ErasingBorder, TransitionBorder };
private:
	enum class ParticleType { Vertex, CircleShape };

	sf::Vector2f m_activeWindowSize;
	sf::Vector2f m_GUIWindowSize;
	tgui::Gui m_gui;

	tgui::Slider::Ptr m_GravitySwitch;
	bool			  m_GravityOn;
	tgui::Slider::Ptr m_FrictionSwitch;
	bool			  m_FrictionOn;
	tgui::Slider::Ptr m_AirResistanceSwitch;
	bool			  m_AirResistanceOn;
	tgui::Slider::Ptr m_WindSwitch;
	bool			  m_WindOn;
	tgui::Knob::Ptr	  m_WindDirection;

	tgui::CheckBox::Ptr m_LifeTime;
	bool			    m_LifeTimeOn;

	tgui::ComboBox::Ptr m_ObjectType;
	tgui::ComboBox::Ptr m_borderType;

	tgui::ListBox::Ptr m_effectType;
	tgui::EditBox::Ptr m_EffectText;

	tgui::RangeSlider::Ptr m_angleRange;
	sf::Vector2f		   m_angleVectorRange;
	tgui::RangeSlider::Ptr m_forceRange;
	sf::Vector2f		   m_forceVectorRange;

	ParticleType	   m_type;
public:
	windowSettings(sf::RenderWindow& window, float border = 500.0f);
	const sf::Vector2f& getActiveWindowSize() { return m_activeWindowSize; }
	const sf::Vector2f& getGUIWindowSize() { return m_GUIWindowSize; }

	std::vector<sf::Vector2f> transitionBorders(const std::vector<sf::Vector2f>&);
	std::vector<size_t> erasingBorders(const std::vector<sf::Vector2f>&);
	std::vector<sf::Vector2f> reboundBorders(const std::vector<sf::Vector2f>&, const std::vector<sf::Vector2f>&);

	void loadGUI();
	void updateGUI(sf::Event& event) { m_gui.handleEvent(event); }
	void updateLogicGUI();
	void drawGUI() { m_gui.draw(); }

	auto& getObjectsUI() { return m_gui.getWidgets(); }

	auto getGravityButton() { return m_gui.get<tgui::Slider>("gravity"); }
	bool getGravityLogic() { return m_GravityOn; }

	auto getFrictionButton() { return m_gui.get<tgui::Slider>("friction"); }
	bool getFrictionLogic() { return m_FrictionOn; }

	auto getAirResistanceButton() { return m_gui.get<tgui::Slider>("airResistance"); }
	bool getAirResistanceLogic() { return m_AirResistanceOn; }

	auto getWindSwitch() { return m_gui.get<tgui::Slider>("windSwitch"); }
	bool getWindLogic() { return m_WindOn; }

	auto getLifeTime() { return m_gui.get<tgui::CheckBox>("lifeTime"); }
	bool getLifeTimeLogic() { return m_LifeTimeOn; }

	auto getWindDirectionKnob() { return m_gui.get<tgui::Knob>("windDirection"); }
	float getWindDirection() { return static_cast<float>(m_WindDirection->getValue()); }

	auto getEffectType() { return m_gui.get<tgui::EditBox>("effectText"); }
	void setEffectType(sf::String effect) { m_EffectText->setText(effect); }

	auto getBorder() { return m_gui.get<tgui::ComboBox>("borderType"); }
	auto getAngleRange() { return m_gui.get<tgui::RangeSlider>("angleRange"); }
	auto getForceRange() { return m_gui.get<tgui::RangeSlider>("forceRange"); }
	sf::Vector2f getForceVectorRance() { return m_forceVectorRange; }

	auto getObjectsType() { return m_gui.get<tgui::ComboBox>("objectType"); }

	ParticleType getParticleType() { return m_type; }
	BorderType getBorderType();
};
