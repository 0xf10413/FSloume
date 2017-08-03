#include "eye.h"
#include "main.h"

Eye::Eye(bool alignLeft)
{
  m_image.create (PUPIL_RADIUS*2, PUPIL_RADIUS*2, alignLeft ? sf::Color::Cyan : sf::Color::Magenta );
  makeADisk(m_image);
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture (m_texture);
}

