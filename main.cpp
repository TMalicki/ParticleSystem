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
                    if (particlesMan.getParticleEffect() == ParticleManage::ParticleEffect::Explode)
                    {
                        particlesMan.explode(mousePosition, sf::Vector2f(-3.0, 3.0), 1000);
                    }
                    else if (particlesMan.getParticleEffect() == ParticleManage::ParticleEffect::Emiter)
                    {
                        particlesMan.createEmiter(mousePosition);
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right)
                {

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
        if (windowSettings.getFrictionLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::Friction);
        if (windowSettings.getAirResistanceLogic() == true) particlesMan.TurnOnForce(true, ParticleSettings::Forces::AirResistance);
        if (windowSettings.getWindLogic() == true)
        {
            float temp = windowSettings.getWindDirection();
            particlesMan.setWindDirection(sf::Vector2f{ -sin(temp * 3.14f / 180.0f), cos(temp * 3.14f / 180.0f) });
            particlesMan.TurnOnForce(true, ParticleSettings::Forces::External);
        }
        particlesMan.setParticleType(static_cast<ParticleManage::ParticleType>(windowSettings.getParticleType()));
        particlesMan.applyFading(windowSettings.getLifeTimeLogic());

        if (windowSettings.getBorderType() == windowSettings::BorderType::ErasingBorder)
        {
            auto& temp = particlesMan.getExplodedParticles();
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

            auto& temp1 = particlesMan.getEmiterParticles();
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