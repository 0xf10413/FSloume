#include "target.h"
#include "config.h"

Target::Target() : m_visible(false)
{
  m_image.create(CG::TARGET_WIDTH, CG::TARGET_HEIGHT, sf::Color(255, 255, 127, 0));
  for (unsigned int i = 0; i < CG::TARGET_WIDTH; ++i)
    m_image.setPixel(i, CG::TARGET_HEIGHT/2, sf::Color(255, 255, 127, 255));
  for (unsigned int j = 0; j < CG::TARGET_HEIGHT; ++j)
    m_image.setPixel(CG::TARGET_WIDTH/2, j, sf::Color(255, 255, 127, 255));
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void Target::updateSprite()
{
  m_visible = true;
  m_sprite.setPosition(
      m_x-(float)CG::TARGET_WIDTH/2,
      m_y-(float)CG::TARGET_HEIGHT/2
      );
}

void Target::hide()
{
  m_visible = false;
}

void Target::setPosition(float x, float y)
{
  m_x = x; m_y = y;
  updateSprite();
}

void Target::draw(sf::RenderWindow &w) const
{
  if (!m_visible)
    return;
  UniqueDrawable::draw(w);
}
