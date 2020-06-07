#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "board.h"

/**
 * Migration class for the game engine.
 * Contains everything to run the game, including the graphic context.
 */
class Game2 : public sf::RenderWindow
{
private:
    Board m_board;
public:
    Game2();
    int mainLoop();
};
