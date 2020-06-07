#include "game2.h"
#include "config.h"
#include "tools/input.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

Game2::Game2() : sf::RenderWindow(sf::VideoMode(CG::WIDTH, CG::HEIGHT), "FSloume")
{
    setFramerateLimit(60);
}

int Game2::mainLoop()
{
    sf::Event event;
    sf::Clock clock;
    sf::RenderTexture renderTexture;

    Input input;
    
    renderTexture.create(getSize().x, getSize().y);

    while (isOpen())
    {
        while(pollEvent(event))
        {
            input.process(event);

            if (event.type == sf::Event::Closed)
                close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    close();
            }
        }

        clear();
        float dt = clock.restart().asSeconds();
        m_board.update(input, dt);
        m_board.render(renderTexture);
        draw(sf::Sprite(renderTexture.getTexture()));
        display();
    }
    return 0;
}
