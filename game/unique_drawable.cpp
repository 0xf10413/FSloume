#include "unique_drawable.h"

UniqueDrawable::UniqueDrawable() :
  m_sprite(), m_image(), m_texture()
{
}

void UniqueDrawable::draw(sf::RenderWindow &w) const
{
  w.draw(m_sprite);
}

MovingEntity::MovingEntity() : UniqueDrawable(),
  m_saved_state{{0,0,0,0,0}},
  m_x(0.), m_y(0.), m_vx(0.), m_vy(0.),
  m_onGround(false)
{

}

void MovingEntity::reinit()
{
  m_x = m_y = m_vx = m_vy = 0.;
  m_onGround = false;
  updateSprite();
}

void MovingEntity::pushState()
{
  m_saved_state = {{m_x, m_y, m_vx, m_vy, (float)m_onGround}};
}

void MovingEntity::popState()
{
  m_x = m_saved_state[0];
  m_y = m_saved_state[1];
  m_vx = m_saved_state[2];
  m_vy = m_saved_state[3];
  m_onGround = (bool)m_saved_state[4];
  updateSprite();
}

bool MovingEntity::getOnGround() const
{
  return m_onGround;
}
