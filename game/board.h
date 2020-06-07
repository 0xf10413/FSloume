#pragma once

#include <SFML/Graphics/RenderTexture.hpp>

#include "entities/slime.h"
#include "entities/ball.h"
#include "entities/net.h"
#include "entities/dangerpt.h"
#include "entities/shockwave.h"

#include "tools/input.h"

/**
 * Class representing an on-going game, complete with characters.
 */
class Board
{
public:
    /**
     * Has someone lost yet ?
     */
    enum class GameState
    {
        PLAYING, // Game is still on-going
        PLAYER1_LOST, // The ball has fallen once on Player1's side
        PLAYER2_LOST, // The ball has fallen once on Player2's side
    };

private:
    // Left and right slime
    Slime m_leftSlime, m_rightSlime;
    
    // One ball bouncing around
    Ball m_ball;

    // A single net to stop the ball and the slimes
    Net m_net;

    // A danger point when the ball is about to reach the ground
    DangerPoint m_dangerPt;

    // A shockwave. Currently mostly unused.
    ShockWave m_shockwave;

    /**
     * @brief Performs an update of all the states after a collision.
     * 
     * @param dt Duration to consider
     * @param fakeUpdate False is this is a simulation update
     */
    void internalUpdate(float dt, bool fakeUpdate);

    /**
     * @brief Computes collisions for an time interval dt.
     * The speeds are assumed already computed.
     * @param dt Time interval to consider.
     */
    void collide(float dt);
public:
    Board();

    /**
     * Update the state of all elements, assuming dt seconds have passed.
     */
    GameState update(const Input &input, float dt);

    /**
     * Renders current state on a texture.
     * The texture is assumed big enough to contain the full board.
     * @param renderTexture Texture to draw to
     */
    void render(sf::RenderTexture &renderTexture);
};
