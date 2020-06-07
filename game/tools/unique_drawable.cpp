#include "tools/unique_drawable.h"

/**********************/

UniqueDrawable::UniqueDrawable() :
  m_sprite(), m_texture()
{
}

void UniqueDrawable::draw(sf::RenderWindow &w) const
{
  w.draw(m_sprite);
}

/************************/
HeavyDrawable::HeavyDrawable() :
  m_image(), m_texture(), m_sprite()
{
}

void HeavyDrawable::draw(sf::RenderWindow &w) const
{
  w.draw(m_sprite);
}

/***********************/

MovingEntity::MovingEntity() : UniqueDrawable(),
  m_saved_state{{0,0,0,0,0}},
  m_x(0.), m_y(0.), m_vx(0.), m_vy(0.),
  m_theta(0.), m_omega(0.),
  m_onGround(false)
{

}


void MovingEntity::reinit()
{
  m_x = m_y = m_vx = m_vy = 0.;
  m_theta = m_omega = 0;
  m_onGround = false;
  updateSprite();
}

void MovingEntity::pushState()
{
  m_saved_state = {{m_x, m_y, m_vx, m_vy, m_theta, m_omega, (float)m_onGround}};
}

void MovingEntity::popState()
{
  m_x = m_saved_state[0];
  m_y = m_saved_state[1];
  m_vx = m_saved_state[2];
  m_vy = m_saved_state[3];
  m_theta = m_saved_state[4];
  m_omega = m_saved_state[5];
  m_onGround = (bool)m_saved_state[6];
  updateSprite();
}

bool MovingEntity::getOnGround() const
{
  return m_onGround;
}

/*******************************/
HeavyMovingEntity::HeavyMovingEntity() : HeavyDrawable(),
  m_saved_state{{0,0,0,0,0}},
  m_x(0.), m_y(0.), m_vx(0.), m_vy(0.),
  m_theta(0.), m_omega(0.),
  m_onGround(false)
{

}


void HeavyMovingEntity::reinit()
{
  m_x = m_y = m_vx = m_vy = 0.;
  m_theta = m_omega = 0;
  m_onGround = false;
  updateSprite();
}

void HeavyMovingEntity::pushState()
{
  m_saved_state = {{m_x, m_y, m_vx, m_vy, m_theta, m_omega, (float)m_onGround}};
}

void HeavyMovingEntity::popState()
{
  m_x = m_saved_state[0];
  m_y = m_saved_state[1];
  m_vx = m_saved_state[2];
  m_vy = m_saved_state[3];
  m_theta = m_saved_state[4];
  m_omega = m_saved_state[5];
  m_onGround = (bool)m_saved_state[6];
  updateSprite();
}

bool HeavyMovingEntity::getOnGround() const
{
  return m_onGround;
}
