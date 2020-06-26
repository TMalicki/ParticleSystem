#include "ParticleSystem.h"
#include <chrono>

float getTime(std::chrono::high_resolution_clock::time_point& start)
{
    auto stop = std::chrono::high_resolution_clock::now();
    auto timePassed = std::chrono::duration<float, std::milli>(stop - start);
    start = stop;

    return  timePassed.count();
}

ParticleSystem::ParticleSystem(sf::Vector2f windowS) : windowSize(windowS), settingWindowWidth(320.0f)
{
    window.create(sf::VideoMode(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)), "ParticleSystem");
    windowSetting = std::make_unique<windowSettings>(window);
    m_particlesManage = std::make_unique<ParticleManage>();

    windowSetting->loadGUI();
    m_particlesManage->setActiveArea(windowSetting->getActiveWindowSize());
    particlesAmount = windowSetting->getAmountDefault();
}

void ParticleSystem::Run()
{
    auto start = std::chrono::high_resolution_clock::now();
    
    while (window.isOpen())
    {
        auto dt = getTime(start);
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        
        // steering ////////////////////////////////////////////////
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            pullParticles(mousePosition);
        }
        ///////////////////////////////////////////////////////////

        // updates ////////////////////////////////////////////////
        sf::Event event;
        while (window.pollEvent(event))
        {
            updateEvent(event, mousePosition);
            windowSetting->updateGUI(event);
        }
        m_particlesManage->setForceRange(windowSetting->getForceVectorRance());
        m_particlesManage->update(dt, mousePosition, std::stoi(particlesAmount));
        windowSetting->updateLogicGUI();
        if (windowSetting->getAmountChanged()) 
        {
            particlesAmount = windowSetting->getAmount();
        }
        ///////////////////////////////////////////////////////////

        applyForces();


        m_particlesManage->setParticleType(static_cast<ParticleManage::ParticleType>(windowSetting->getParticleType()));
        m_particlesManage->applyFading(windowSetting->getLifeTimeLogic());

        applyBorders();

        m_particlesManage->colorParticlesByVelocity(m_particlesManage->getExplodedParticles());
        m_particlesManage->colorParticlesByVelocity(m_particlesManage->getEmiterParticles());
        m_particlesManage->colorParticlesByVelocity(m_particlesManage->getTunnelParticles());

        // drawings //////////////////////////////////////////////
        window.clear();
        m_particlesManage->draw(window);
        windowSetting->drawGUI();
        window.display();
        //////////////////////////////////////////////////////////
    }
}

void ParticleSystem::applyForces()
{
    if (windowSetting->getGravityLogic() == true) m_particlesManage->TurnOnForce(true, ParticleSettings::Forces::Gravity);
    else { m_particlesManage->TurnOnForce(false, ParticleSettings::Forces::Gravity); }
    if (windowSetting->getFrictionLogic() == true) m_particlesManage->TurnOnForce(true, ParticleSettings::Forces::Friction);
    else { m_particlesManage->TurnOnForce(false, ParticleSettings::Forces::Friction); }
    if (windowSetting->getAirResistanceLogic() == true) m_particlesManage->TurnOnForce(true, ParticleSettings::Forces::AirResistance);
    else { m_particlesManage->TurnOnForce(false, ParticleSettings::Forces::AirResistance); }
    if (windowSetting->getWindLogic() == true)
    {
        float temp = windowSetting->getWindDirection();
        m_particlesManage->setWindDirection(sf::Vector2f{ -sin(temp * 3.14f / 180.0f), cos(temp * 3.14f / 180.0f) });
        m_particlesManage->TurnOnForce(true, ParticleSettings::Forces::Wind);
    }
    else { m_particlesManage->TurnOnForce(false, ParticleSettings::Forces::Wind); }
}

void ParticleSystem::erasingBorder(ParticleManage::ParticleEffect effect)
{
    std::unique_ptr<std::vector<std::shared_ptr<ParticlesInterface>>> temp;
    if (effect == ParticleManage::ParticleEffect::Explode)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getExplodedParticles());
    }
    else if (effect == ParticleManage::ParticleEffect::Emiter)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getEmiterParticles());
    }
    else if (effect == ParticleManage::ParticleEffect::Tunnel)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getTunnelParticles());
    }

    //auto& temp = m_particlesManage->getExplodedParticles();
    std::vector<size_t> particleGroupToErase;
    std::vector<std::vector<size_t>> particlesID;

    for (size_t i = 0; i < temp->size(); i++)
    {
        particlesID.push_back(windowSetting->erasingBorders(temp->at(i)->getPosition()));
    }
    particleGroupToErase = m_particlesManage->eraseParticles(*temp, particlesID); // zamiast temp dać ParticleManage::ParticleEffect::Explode           
    m_particlesManage->eraseParticlesGroup(*temp, particleGroupToErase);
}

void ParticleSystem::reboundBorder(ParticleManage::ParticleEffect effect)
{
    std::unique_ptr<std::vector<std::shared_ptr<ParticlesInterface>>> temp;

    if (effect == ParticleManage::ParticleEffect::Explode)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getExplodedParticles());
    }
    else if (effect == ParticleManage::ParticleEffect::Emiter)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getEmiterParticles());
    }
    else if (effect == ParticleManage::ParticleEffect::Tunnel)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getTunnelParticles());
    }

    for (size_t i = 0; i < temp->size(); i++)
    {
        (*temp)[i]->setVelocity(windowSetting->reboundBorders((*temp)[i]->getPosition(), (*temp)[i]->getVelocity()));
    }    
}

void ParticleSystem::transitionBorder(ParticleManage::ParticleEffect effect)
{
    std::unique_ptr<std::vector<std::shared_ptr<ParticlesInterface>>> temp;

    if (effect == ParticleManage::ParticleEffect::Explode)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getExplodedParticles());
    }
    else if (effect == ParticleManage::ParticleEffect::Emiter)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getEmiterParticles());
    }
    else if (effect == ParticleManage::ParticleEffect::Tunnel)
    {
        temp = std::make_unique<std::vector<std::shared_ptr<ParticlesInterface>>>(m_particlesManage->getTunnelParticles());
    }

    for (size_t i = 0; i < temp->size(); i++)
    {
        (*temp)[i]->setPosition(windowSetting->transitionBorders((*temp)[i]->getPosition()));
    }
}

void ParticleSystem::applyBorders()
{
    if (windowSetting->getBorderType() == windowSettings::BorderType::ErasingBorder)
    {
        if (m_particlesManage->getExplodedParticles().size() > 0)
        {
            erasingBorder(ParticleManage::ParticleEffect::Explode);
        }
        if (m_particlesManage->getEmiterParticles().size() > 0)
        {
            erasingBorder(ParticleManage::ParticleEffect::Emiter);
        }
        if (m_particlesManage->getTunnelParticles().size() > 0)
        {
            erasingBorder(ParticleManage::ParticleEffect::Tunnel);
        }
    }
    else if (windowSetting->getBorderType() == windowSettings::BorderType::ReboundBorder)
    {
        if (m_particlesManage->getExplodedParticles().size() > 0)
        {
            reboundBorder(ParticleManage::ParticleEffect::Explode);
        }
        if (m_particlesManage->getEmiterParticles().size() > 0)
        {
            reboundBorder(ParticleManage::ParticleEffect::Emiter);
        }
        if (m_particlesManage->getTunnelParticles().size() > 0)
        {
            reboundBorder(ParticleManage::ParticleEffect::Tunnel);
        }
    }
    else if (windowSetting->getBorderType() == windowSettings::BorderType::TransitionBorder)
    {
        if (m_particlesManage->getExplodedParticles().size() > 0)
        {
            transitionBorder(ParticleManage::ParticleEffect::Explode);
        }
        if (m_particlesManage->getEmiterParticles().size() > 0)
        {
            transitionBorder(ParticleManage::ParticleEffect::Emiter);
        }
        if (m_particlesManage->getTunnelParticles().size() > 0)
        {
            transitionBorder(ParticleManage::ParticleEffect::Tunnel);
        }
    }
}

void ParticleSystem::pullParticles(sf::Vector2i mousePosition)
{
    for (auto& particle : m_particlesManage->getExplodedParticles())
    {
        particle->setDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
        particle->applyForce(sf::Vector2f{ 0.1f , 0.1f });
    }
    for (auto& particle : m_particlesManage->getEmiterParticles())
    {
        particle->setDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
        particle->applyForce(sf::Vector2f{ 0.1f , 0.1f });
    }
    for (auto& particle : m_particlesManage->getTunnelParticles())
    {
        particle->setDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
        particle->applyForce(sf::Vector2f{ 0.1f , 0.1f });
    }
}

void ParticleSystem::chooseEffect(float mouseDelta)
{
    int temp = static_cast<int>(m_particlesManage->getParticleEffect());
    int max = static_cast<int>((ParticleManage::ParticleEffect::NUMBER_OF_EFFECTS));
    if (mouseDelta > 0)
    {
        if (temp < (max - 1))
        {
            m_particlesManage->setEffectType(static_cast<ParticleManage::ParticleEffect>((temp + 1)));
        }
        else
        {
            m_particlesManage->setEffectType(static_cast<ParticleManage::ParticleEffect>((0)));
        }
    }
    else if (mouseDelta < 0)
    {
        if (temp > 0)
        {
            m_particlesManage->setEffectType(static_cast<ParticleManage::ParticleEffect>((temp - 1)));
        }
        else
        {
            m_particlesManage->setEffectType(static_cast<ParticleManage::ParticleEffect>((max - 1)));
        }
    }
    windowSetting->setEffectType(m_particlesManage->getEffectText());
}

void ParticleSystem::updateEvent(sf::Event& event, sf::Vector2i mousePosition)
{
    if (event.type == sf::Event::Closed)
        window.close();
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            if (m_particlesManage->getParticleEffect() == ParticleManage::ParticleEffect::Explode)
            {
                m_particlesManage->TurnEffectOn(ParticleManage::ParticleEffect::Explode);
            }
            else if (m_particlesManage->getParticleEffect() == ParticleManage::ParticleEffect::Emiter)
            {
                m_particlesManage->createEmitingObject(mousePosition, 20.0f, std::stoi(particlesAmount));
                //m_particlesManage->TurnEffectOn(ParticleManage::ParticleEffect::Emiter); 
            }
            
            else if (m_particlesManage->getParticleEffect() == ParticleManage::ParticleEffect::Tunnel)
            {
                if (m_particlesManage->getTunnelEffectLogic() == false)
                    m_particlesManage->TurnEffectOn(ParticleManage::ParticleEffect::Tunnel);
                else
                    m_particlesManage->setTunnelEffectLogic(false);
            }
            
        }
    }
    else if (event.type == sf::Event::MouseWheelScrolled)
    {
        chooseEffect(event.mouseWheelScroll.delta);
    }
}

