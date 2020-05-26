#include <iostream>
#include <TGUI/TGUI.hpp>
#include "windowSettings.h"

#include "ParticleManage.h"
#include "Timer.h"

// turn on randomness walk particle - needed
// after all particles on the screen erased and new explosion wanna be made - error occured
// friction should be working only in y direction? Or should i choose in which direction that works (x and y, or just one of them)
// emiter sometimes is erasing while using erasingBorders when it should not happen
// also - emiter should be sigle particle at the time, it should have life spawn and it should have rateSpawn method
// void ParticleManage::applyFading() ----- do template of that?

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    sf::Vector2f windowSize = sf::Vector2f{ 1920.0f, 1080.0f };
    float settingWindowWidth = 320;

    sf::RenderWindow window(sf::VideoMode(static_cast<int>(windowSize.x), static_cast<int>(windowSize.y)), "ParticleSystem");
    windowSettings windowSettings(window);
    windowSettings.loadGUI();


     ParticleManage particlesMan;
     particlesMan.setActiveArea(windowSettings.getActiveWindowSize());

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
                windowSettings.setEffectType(particlesMan.getEffectText());
             }
            windowSettings.updateGUI(event);
        }
        particlesMan.update(dt);
        windowSettings.updateLogicGUI();

        ///////////////////////////////////////////
        /// TO Z WINDOW SETTINGS/////
        //////////////////////////////////////////
        if(windowSettings.getGravityLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::Gravity);
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::Gravity); }
        if (windowSettings.getFrictionLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::Friction);
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::Friction); }
        if (windowSettings.getAirResistanceLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::AirResistance);
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::AirResistance); }
        if (windowSettings.getWindLogic() == true)
        {
            float temp = windowSettings.getWindDirection();
            particlesMan.setWindDirection(sf::Vector2f{ -sin(temp * 3.14f / 180.0f), cos(temp * 3.14f / 180.0f) });
            particlesMan.TurnOnForce(true, ParticleSettings::Forces::Wind);
        }
        else { particlesMan.TurnOnForce(false, ParticleSettings::Forces::Wind); }

        particlesMan.setParticleType(static_cast<ParticleManage::ParticleType>(windowSettings.getParticleType()));
        particlesMan.applyFading(windowSettings.getLifeTimeLogic());

        if (windowSettings.getBorderType() == windowSettings::BorderType::ErasingBorder)
        { 
            auto& temp = particlesMan.getExplodedParticles();
            std::vector<size_t> particleGroupToErase;
            std::vector<std::vector<size_t>> particlesID;

            for (size_t i = 0; i < temp.size(); i++)
            {
                particlesID.push_back(windowSettings.erasingBorders(temp.at(i)->getPosition()));
            }
            particleGroupToErase = particlesMan.eraseParticles(temp, particlesID); // zamiast temp dać ParticleManage::ParticleEffect::Explode           
            particlesMan.eraseParticlesGroup(temp, particleGroupToErase);
            
            auto& temp1 = particlesMan.getEmiterParticles();
            std::vector<size_t> particleGroupToErase1;
            std::vector<std::vector<size_t>> particlesID1;

            for (size_t i = 0; i < temp1.size(); i++)
            {
                particlesID1.push_back(windowSettings.erasingBorders(temp1.at(i)->getPosition()));
            }
            particleGroupToErase1 = particlesMan.eraseParticles(temp1, particlesID1);
            particlesMan.eraseParticlesGroup(temp1, particleGroupToErase1);
        }
        else if (windowSettings.getBorderType() == windowSettings::BorderType::ReboundBorder)
        {
            auto& temp = particlesMan.getExplodedParticles();
            for (size_t i = 0; i < temp.size(); i++)
            {
                temp[i]->setVelocity(windowSettings.reboundBorders(temp[i]->getPosition(), temp[i]->getVelocity()));
            }

            auto& temp1 = particlesMan.getEmiterParticles();
            for (size_t i = 0; i < temp1.size(); i++)
            {
                temp1[i]->setVelocity(windowSettings.reboundBorders(temp1[i]->getPosition(), temp1[i]->getVelocity()));
            }
        }
        else if (windowSettings.getBorderType() == windowSettings::BorderType::TransitionBorder)
        {
            auto& temp = particlesMan.getExplodedParticles();
            for (size_t i = 0; i < temp.size(); i++)
            {
                temp[i]->setPosition(windowSettings.transitionBorders(temp[i]->getPosition()));
            }

            auto& temp1 = particlesMan.getEmiterParticles();
            for (size_t i = 0; i < temp1.size(); i++)
            {
                temp1[i]->setPosition(windowSettings.transitionBorders(temp1[i]->getPosition()));
            }
        }

        ///////////////////////////////////////////////
        //////////////////////////////////
        ////////////////////////////////////////////

        particlesMan.colorParticlesByVelocity(particlesMan.getExplodedParticles());
        particlesMan.colorParticlesByVelocity(particlesMan.getEmiterParticles());

        window.clear();

        particlesMan.draw(window);  

        windowSettings.drawGUI();
        window.display();
    }
    return 0;
}