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
}

void ParticleSystem::Run()
{
    auto start = std::chrono::high_resolution_clock::now();
    

    //windowSettings windowSettings(window);
    windowSetting->loadGUI();


    ParticleManage particlesMan;
    particlesMan.setActiveArea(windowSetting->getActiveWindowSize());

    while (window.isOpen())
    {
        auto dt = getTime(start);
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            for (auto& particle : particlesMan.getExplodedParticles())
            {
                particle->setDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
                particle->applyForce(sf::Vector2f{ 0.1f , 0.1f });
            }
            for (auto& particle : particlesMan.getEmiterParticles())
            {
                particle->setDirectionTowardsPoint(static_cast<sf::Vector2f>(mousePosition));
                particle->applyForce(sf::Vector2f{ 0.1f , 0.1f });
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (particlesMan.getParticleEffect() == ParticleManage::ParticleEffect::Explode) // it should be take from windowSetting class
                    {
                        particlesMan.applyEffect(ParticleManage::ParticleEffect::Explode, mousePosition, sf::Vector2f(-3.0, 3.0), 1000);
                    }
                    else if (particlesMan.getParticleEffect() == ParticleManage::ParticleEffect::Emiter)
                    {
                        particlesMan.createEmitingObject(mousePosition);    // maybe make it somehow like with explode method?
                    }
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled)
            {
                int temp = static_cast<int>(particlesMan.getParticleEffect());
                int max = static_cast<int>((ParticleManage::ParticleEffect::NUMBER_OF_EFFECTS));
                if (event.mouseWheelScroll.delta > 0)
                {
                    if (temp < (max - 1))
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((temp + 1)));
                    }
                    else
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((0)));
                    }
                }
                else if (event.mouseWheelScroll.delta < 0)
                {
                    if (temp > 0)
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((temp - 1)));
                    }
                    else
                    {
                        particlesMan.setEffectType(static_cast<ParticleManage::ParticleEffect>((max - 1)));
                    }
                }
                windowSetting->setEffectType(particlesMan.getEffectText());
            }
            windowSetting->updateGUI(event);
        }
        particlesMan.update(dt);
        windowSetting->updateLogicGUI();

        ///////////////////////////////////////////
        /// TO Z WINDOW SETTINGS/////
        //////////////////////////////////////////
        if (windowSetting->getGravityLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::Gravity);
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::Gravity); }
        if (windowSetting->getFrictionLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::Friction);
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::Friction); }
        if (windowSetting->getAirResistanceLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::AirResistance);
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::AirResistance); }
        if (windowSetting->getWindLogic() == true)
        {
            float temp = windowSetting->getWindDirection();
            particlesMan.setWindDirection(sf::Vector2f{ -sin(temp * 3.14f / 180.0f), cos(temp * 3.14f / 180.0f) });
            particlesMan.TurnOnForce(true, ParticleSettings::Forces::Wind);
        }
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::Wind); }

        particlesMan.setParticleType(static_cast<ParticleManage::ParticleType>(windowSetting->getParticleType()));
        particlesMan.applyFading(windowSetting->getLifeTimeLogic());

        if (windowSetting->getBorderType() == windowSettings::BorderType::ErasingBorder)
        {
            auto& temp = particlesMan.getExplodedParticles();
            std::vector<size_t> particleGroupToErase;
            std::vector<std::vector<size_t>> particlesID;

            for (size_t i = 0; i < temp.size(); i++)
            {
                particlesID.push_back(windowSetting->erasingBorders(temp.at(i)->getPosition()));
            }
            particleGroupToErase = particlesMan.eraseParticles(temp, particlesID); // zamiast temp dać ParticleManage::ParticleEffect::Explode           
            particlesMan.eraseParticlesGroup(temp, particleGroupToErase);

            auto& temp1 = particlesMan.getEmiterParticles();
            std::vector<size_t> particleGroupToErase1;
            std::vector<std::vector<size_t>> particlesID1;

            for (size_t i = 0; i < temp1.size(); i++)
            {
                particlesID1.push_back(windowSetting->erasingBorders(temp1.at(i)->getPosition()));
            }
            particleGroupToErase1 = particlesMan.eraseParticles(temp1, particlesID1);
            particlesMan.eraseParticlesGroup(temp1, particleGroupToErase1);
        }
        else if (windowSetting->getBorderType() == windowSettings::BorderType::ReboundBorder)
        {
            auto& temp = particlesMan.getExplodedParticles();
            for (size_t i = 0; i < temp.size(); i++)
            {
                temp[i]->setVelocity(windowSetting->reboundBorders(temp[i]->getPosition(), temp[i]->getVelocity()));
            }

            auto& temp1 = particlesMan.getEmiterParticles();
            for (size_t i = 0; i < temp1.size(); i++)
            {
                temp1[i]->setVelocity(windowSetting->reboundBorders(temp1[i]->getPosition(), temp1[i]->getVelocity()));
            }
        }
        else if (windowSetting->getBorderType() == windowSettings::BorderType::TransitionBorder)
        {
            auto& temp = particlesMan.getExplodedParticles();
            for (size_t i = 0; i < temp.size(); i++)
            {
                temp[i]->setPosition(windowSetting->transitionBorders(temp[i]->getPosition()));
            }

            auto& temp1 = particlesMan.getEmiterParticles();
            for (size_t i = 0; i < temp1.size(); i++)
            {
                temp1[i]->setPosition(windowSetting->transitionBorders(temp1[i]->getPosition()));
            }
        }

        ///////////////////////////////////////////////
        //////////////////////////////////
        ////////////////////////////////////////////

        particlesMan.colorParticlesByVelocity(particlesMan.getExplodedParticles());
        particlesMan.colorParticlesByVelocity(particlesMan.getEmiterParticles());

        window.clear();

        particlesMan.draw(window);

        windowSetting->drawGUI();
        window.display();
    }
}