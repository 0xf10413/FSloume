#include "board.h"
#include "tools/utils.h"
#include "tools/ia.h"

Board::Board():
    m_leftSlime(true), m_rightSlime(false),
    m_shockwave(true)
{
    m_leftSlime.setMainCharacter(true);
    m_rightSlime.setMainCharacter(false);

    m_leftSlime.reinit();
    m_leftSlime.setX(CG::WIDTH/4);
    m_leftSlime.setY(CG::HEIGHT);
    m_leftSlime.clampTo(sf::FloatRect(0, 0, CG::WIDTH/2 - CG::NET_WIDTH/2, CG::HEIGHT));

    m_rightSlime.reinit();
    m_rightSlime.setX (3*CG::WIDTH/4);
    m_rightSlime.setY (CG::HEIGHT);
    m_rightSlime.clampTo(sf::FloatRect(CG::WIDTH/2 + CG::NET_WIDTH/2, 0, CG::WIDTH/2 - CG::NET_WIDTH/2, CG::HEIGHT));

    m_ball.reinit();
    m_ball.setX(CG::WIDTH/4);
    m_ball.setY(CG::HEIGHT/2);
    m_ball.clampTo(sf::FloatRect(0, 0, CG::WIDTH, CG::HEIGHT));

    m_net.reinit();
    m_net.setPosition(CG::WIDTH/2, CG::HEIGHT - CG::NET_HEIGHT/2);
}

Board::GameState Board::update(const Input &input, float dt)
{
    m_leftSlime.prepareMove(input);
    IA(IA::Difficulty::NORMAL).interact(m_rightSlime, m_ball, m_dangerPt.getPosition(),
            m_shockwave);
    //m_rightSlime.prepareMove(input);

    m_leftSlime.pushState();
    m_rightSlime.pushState();
    m_ball.pushState();

    // Run the simulation and update the dangerpoint
    m_dangerPt.setPosition({-100, -100});
    for (int i = 0; i < CG::BALL_ANTICIPATION; ++i)
    {
        internalUpdate(dt, true);
        m_ball.updatePath(i);
        if (m_ball.getOnGround())
        {
            m_dangerPt.setPosition(m_ball.getPosition());
            break;
        }
    }

    m_leftSlime.popState();
    m_rightSlime.popState();
    m_ball.popState();

    internalUpdate(dt, false);

    if (m_ball.getOnGround())
    {
        if (m_ball.getPosition().x <= CG::WIDTH/2)
        {
            std::cout << "Player 1 lost!" << std::endl;
            return GameState::PLAYER1_LOST;
        }
        else
        {
            std::cout << "Player 2 lost!" << std::endl;
            return GameState::PLAYER2_LOST;
        }
    }
    return GameState::PLAYING;
}

void Board::internalUpdate(float dt, bool fakeUpdate)
{
    collide(dt);
    m_leftSlime.move(dt, m_ball, fakeUpdate);
    m_rightSlime.move(dt, m_ball, fakeUpdate);
    m_ball.move(dt);
}

void Board::collide(float dt)
{
    /* Left slime and ball */
    float t = collideTwoCircles(
        m_leftSlime.getPosition(), CG::SLIME_HEIGHT, m_leftSlime.getSpeed(),
        m_ball.getPosition(), CG::BALL_RADIUS, m_ball.getSpeed(), dt);

    if (t >= 0.)
    {
        sf::Vector2f v1 = m_leftSlime.getSpeed();
        sf::Vector2f x1 = m_leftSlime.getPosition() + t*v1;
        sf::Vector2f v2 = m_ball.getSpeed();
        sf::Vector2f x2 = m_ball.getPosition() + t*v2;

        sf::Vector2f deltaX = x2 - x1;
        sf::Vector2f v2p = v2 - 2*dotProduct(v2 - v1, deltaX)
            *deltaX/abs2(deltaX);
        v2p *= CG::BALL_ELASTICITY*CG::SLIME_ELASTICITY;
        m_ball.setSpeed(v2p);
    }

    /* Right slime and ball */
    t = collideTwoCircles(
        m_rightSlime.getPosition(), CG::SLIME_HEIGHT, m_rightSlime.getSpeed(),
        m_ball.getPosition(), CG::BALL_RADIUS, m_ball.getSpeed(), dt);
    if (t >= 0.) // Intersection !
    {
        sf::Vector2f v1 = m_rightSlime.getSpeed();
        sf::Vector2f x1 = m_rightSlime.getPosition() + t*v1;
        sf::Vector2f v2 = m_ball.getSpeed();
        sf::Vector2f x2 = m_ball.getPosition() + t*v2;

        sf::Vector2f deltaX = x2 - x1;
        sf::Vector2f v2p = v2 - 2*dotProduct(v2 - v1, deltaX)
            *deltaX/abs2(deltaX);
        v2p *= CG::BALL_ELASTICITY*CG::SLIME_ELASTICITY;
        m_ball.setSpeed(v2p);
    }


    /* Crush shielding */
    m_ball.setSpeed(uncrushCircleFromRecAndCircle(m_ball.getPosition(), CG::BALL_RADIUS,
        m_ball.getSpeed(), m_leftSlime.getPosition(), CG::SLIME_WIDTH/2));
    m_ball.setSpeed(uncrushCircleFromRecAndCircle(m_ball.getPosition(), CG::BALL_RADIUS,
        m_ball.getSpeed(), m_rightSlime.getPosition(), CG::SLIME_WIDTH/2));

    /* Net and ball */
    sf::Vector2f bounce = collideWithFixRectangle(
        m_net.getBox(),
        m_ball.getPosition(), CG::BALL_RADIUS,m_ball.getSpeed(),  dt
        );

    if (abs2(bounce) != 0.)
    {
        auto speed = m_ball.getSpeed();
        if (bounce.x != 0.)
            speed.x = copysignf(speed.x, bounce.x);
        if (bounce.y != 0.)
            speed.y = copysignf(speed.y, bounce.y);
        m_ball.setSpeed(speed);
    }
}

void Board::render(sf::RenderTexture &renderTexture)
{
    renderTexture.clear();

    m_leftSlime.draw(renderTexture);
    m_rightSlime.draw(renderTexture);
    m_net.draw(renderTexture);
    m_ball.draw(renderTexture);
    m_dangerPt.draw(renderTexture);

    renderTexture.display();
}
