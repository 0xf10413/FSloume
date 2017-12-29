#include "eye.h"
#include "config.h"
#include "utils.h"
#include "rc_manager.h"

Eye::Eye(bool alignLeft)
{
  m_texture = ResourceManager::getTexture(alignLeft ? "bEye" : "rEye");
  m_sprite.setTexture (*m_texture.lock());
}

void Eye::setPosition (float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

void Eye::updateSprite ()
{
  m_sprite.setPosition(
      m_x - (float)CG::PUPIL_RADIUS,
      m_y - (float)CG::PUPIL_RADIUS
      );
}

/* Décale l'œil pour regarder le point at*/
/* Suppose l'œil remis au centre avant */
void Eye::lookAt(const sf::Vector2f &at)
{
  sf::Vector2f delta = at - sf::Vector2f(m_x, m_y);
  delta *= (CG::EYE_RADIUS-(float)CG::PUPIL_RADIUS)/norm2(delta);
  m_x += delta.x;
  m_y += delta.y;

  updateSprite();
}
