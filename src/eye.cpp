#include "eye.h"
#include "main.h"

Eye::Eye(bool alignLeft)
{
  m_image.create (PUPIL_RADIUS*2, PUPIL_RADIUS*2, alignLeft ? sf::Color::Cyan : sf::Color::Magenta );
  makeADisk(m_image);
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture (m_texture);
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
      m_x - PUPIL_RADIUS,
      m_y - PUPIL_RADIUS
      );
}

/* Décale l'œil pour regarder le point at*/
/* Suppose l'œil remis au centre avant */
void Eye::lookAt(const sf::Vector2f &at)
{
  sf::Vector2f delta = at - sf::Vector2f(m_x, m_y);
  delta *= (EYE_RADIUS-PUPIL_RADIUS)/norm2(delta);
  m_x += delta.x;
  m_y += delta.y;

  updateSprite();
}
