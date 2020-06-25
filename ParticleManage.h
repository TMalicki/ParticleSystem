#pragma once
#include <iostream>
#include <random>
#include "ParticlesInterface.h"
#include "ParticlesVertex.h"
#include "ParticlesCircle.h"
#include "EmiterEffect.h"
#include "windowSettings.h"


class ParticleManage
{
public:
	enum class ParticleType { Vertex, CircleShape };
	enum class ParticleEffect { Explode, Emiter, Tunnel, NUMBER_OF_EFFECTS };
private:
	std::default_random_engine									m_generator;
	sf::Vector2f												m_activeArea;


	// particles group //

	std::vector<std::vector<std::shared_ptr<ParticlesInterface>>>	m_particleContainer;
	std::vector<std::shared_ptr<ParticlesInterface>>			m_explodedParticles;
	std::vector<std::shared_ptr<ParticlesInterface>>			m_emiterParticles;
	std::vector<std::shared_ptr<ParticlesInterface>>			m_tunnelParticles;

	std::vector<sf::CircleShape>								m_force;
	sf::Vector2f												m_forceRange;

	// forces //
	bool														m_FrictionOn;
	bool														m_GravityOn;
	bool														m_AirResistanceOn;
	bool														m_WindOn;
	sf::Vector2f												m_WindDirection;
	float														m_forceWaveForce;

	// with GUI connected
	ParticleType												m_type;
	ParticleEffect												m_effectType;

	// effects
	bool														m_fadingOn;
	bool														m_explodedParticlesOn;
	bool														m_emiterParticlesOn;
	bool														m_tunnelParticlesOn;
	EmiterEffect												emiterEffect;

public:
	ParticleManage() : m_forceWaveForce{ 0.0f }, m_FrictionOn{ false }, m_GravityOn{ false }, m_AirResistanceOn{ false }, m_WindOn{ false }, m_type{ ParticleType::Vertex }
		, m_effectType{ ParticleEffect::Explode }, emiterEffect{}, m_fadingOn{ false }, m_forceRange{ sf::Vector2f{0.0f,3.0f} }, m_explodedParticlesOn{ false }
		, m_emiterParticlesOn{ false }
	{
		m_particleContainer.push_back(m_explodedParticles);
		m_particleContainer.push_back(m_emiterParticles);
		m_particleContainer.push_back(m_tunnelParticles);
	}
	//ParticleManage(const ParticleManage&) { std::cout << "ParticleManage kopia"; };

	void setActiveArea(sf::Vector2f area) { m_activeArea = area; } 
	void setParticleType(ParticleType type) { m_type = type; }	// that is not needed when ParticleManage class will have windowSetting class

	void createEmitingObject(sf::Vector2i, float, int);

	void applyEffect(ParticleEffect effect, sf::Vector2i mousePosition, sf::Vector2f forceRange, sf::Vector2f angleRange, int amount = 1000);
	void TurnEffectOn(ParticleEffect effect);

	std::vector<size_t> eraseParticles(std::vector<std::shared_ptr<ParticlesInterface>>&, std::vector<std::vector<size_t>> elementsID);
	void eraseParticlesGroup(std::vector<std::shared_ptr<ParticlesInterface>>&, std::vector<size_t> toEraseGroup);

	void applyFading(bool logic);
	void updateFading(float dt);
	bool getTunnelEffectLogic() { return m_tunnelParticlesOn; }
	void setTunnelEffectLogic(bool logic) { m_tunnelParticlesOn = logic; }

	void colorParticlesByVelocity(std::vector<std::shared_ptr<ParticlesInterface>>& particles);

	void setEffectType(ParticleEffect effect) { m_effectType = effect; }
	const ParticleEffect& getParticleEffect() { return m_effectType; }

	void createParticles(std::vector<std::shared_ptr<ParticlesInterface>>&, sf::Vector2i mousePosition = sf::Vector2i(0, 0), int amount = 1000);
	void setParticleExpandAttributes(std::vector<std::shared_ptr<ParticlesInterface>>&, sf::Vector2i, std::vector<sf::Vector2f> direction, sf::Vector2f randomRange = sf::Vector2f(0.0f, 0.0f));
	
	std::vector<std::shared_ptr<ParticlesInterface>>& getExplodedParticles() { return m_particleContainer[0]; }
	std::vector<std::shared_ptr<ParticlesInterface>>& getEmiterParticles() { return m_particleContainer[1]; }

	void setWindDirection(sf::Vector2f direction) { m_WindDirection = direction; }
	void setForceRange(sf::Vector2f);
	sf::Vector2f getForceRange() { return m_forceRange; }

	void particlePush(const std::vector<std::tuple<size_t, size_t, size_t>>&, bool collision = false);
	void createForceWave(sf::Vector2i, float radius = 1.0f);
	void forceWaveExpand(float, sf::Vector2f);
	const auto isForceWaveCollided();

	void setWaveForce(float force) { m_forceWaveForce = force; }
	const float getWaveForce() { return m_forceWaveForce; }

	void TurnOnForce(bool, ParticleSettings::Forces);
	void updateForce();
	void updatePosition(float dt);
	void applyWindForce(sf::Vector2f force);
	void applyGravityForce(sf::Vector2f force);
	void applyAirResistance(float coefficent = 0.0001f);
	void applyFriction(float mi = 0.01);

	sf::String getEffectText();

	float getRandomFloat(float, float);
	void update(float dt, sf::Vector2i, int);
	void draw(sf::RenderWindow&);
};