#include "target.h"
#include "config.h"

Target::Target()
{
  m_image.create(WIDTH, HEIGHT, sf::Color(255, 255, 127, 0));
  for (int i = 0; i < WIDTH; ++i)
    m_image.setPixel(i, HEIGHT/2, sf::Color(255, 255, 127, 255));
  for (int j = 0; j < HEIGHT; ++j)
    m_image.setPixel(WIDTH/2, j, sf::Color(255, 255, 127, 255));
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void Target::updateSprite()
{
  m_sprite.setPosition(m_x-WIDTH/2, m_y-HEIGHT/2);
}

void Target::setPosition(float x, float y)
{
  m_x = x; m_y = y;
  updateSprite();
}
