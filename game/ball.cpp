#include "ball.h"
#include "utils.h"

Ball::Ball() :
  MovingEntity(),
  m_clamp()
{
  m_image.create(BALL_RADIUS*2, BALL_RADIUS*2, sf::Color::Red);
  makeADisk(m_image);
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void Ball::setX (float x)
{
  m_x = x;
  updateSprite();
}

void Ball::setY (float y)
{
  m_y = y;
  updateSprite();
}

void Ball::clampTo (const sf::FloatRect &clamp)
{
  m_clamp = clamp;
}

void Ball::move (float dt)
{
  /* Déplacement avec la vitesse */
  m_x += dt*m_vx;
  m_y += dt*m_vy;

  /* Perte de vitesse avec la gravité */
  m_vy += GRAVITY*dt;

  if (m_x < m_clamp.left + BALL_RADIUS)
  {
    m_x = m_clamp.left + BALL_RADIUS;
    m_vx = -m_vx*BALL_ELASTICITY;
  }

  if (m_x > m_clamp.left + m_clamp.width - BALL_RADIUS)
  {
    m_x = m_clamp.left + m_clamp.width - BALL_RADIUS;
    m_vx = -m_vx*BALL_ELASTICITY;
  }

  if (m_y > m_clamp.top + m_clamp.height - BALL_RADIUS)
  {
    m_y = m_clamp.top + m_clamp.height - BALL_RADIUS;
    m_vy = -m_vy*BALL_ELASTICITY;
  }

  updateSprite();
}

void Ball::updateSprite()
{
  m_sprite.setPosition(
      m_x - BALL_RADIUS,
      m_y - BALL_RADIUS
      );
}

sf::Vector2f Ball::getPosition() const
{
  return {m_x, m_y};
}

sf::Vector2f Ball::getSpeed() const
{
  return {m_vx, m_vy};
}

void Ball::setSpeed(const sf::Vector2f &v)
{
  m_vx = v.x;
  m_vy = v.y;
}
