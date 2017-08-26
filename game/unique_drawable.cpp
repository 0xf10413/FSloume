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
  m_x(0.), m_y(0.), m_vx(0.), m_vy(0.)
{

}

void MovingEntity::reinit()
{
  m_x = m_y = m_vx = m_vy = 0.;
  updateSprite();
}
