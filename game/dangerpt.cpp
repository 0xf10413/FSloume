/*
 * dangerpt.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "dangerpt.h"
#include "config.h"


DangerPoint::DangerPoint(const sf::Color &c) : MovingEntity(), m_color(c)
{
  m_image.create(DANGERPT_WIDTH, DANGERPT_WIDTH, m_color);
  for (int i = 0; i < DANGERPT_WIDTH; ++i)
    for (int j = 0; j < DANGERPT_WIDTH; ++j)
    {
      if (-2*i + DANGERPT_WIDTH > j ||
          2*i - DANGERPT_WIDTH > j)
        m_image.setPixel(i,j,sf::Color(0,0,0,0));
    }
  for (int i = DANGERPT_WIDTH/10; i < 7*DANGERPT_WIDTH/10; ++i)
    for (int j = DANGERPT_WIDTH/2-1; j < DANGERPT_WIDTH/2+2; ++j)
      m_image.setPixel(j, i, sf::Color::White);
  for (int i = DANGERPT_WIDTH/2-2; i < DANGERPT_WIDTH/2+3; ++i)
    for (int j = 9*DANGERPT_WIDTH/10-2; j < 9*DANGERPT_WIDTH/10+3; ++j)
    m_image.setPixel(i, j, sf::Color::White);
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void DangerPoint::updateSprite()
{
  m_sprite.setPosition(
      m_x - BALL_RADIUS,
      m_y - BALL_RADIUS
      );
}

void DangerPoint::setPosition(const sf::Vector2f &v)
{
  m_x = v.x;
  m_y = v.y;
  updateSprite();
}

sf::Vector2f DangerPoint::getPosition()
{
  return {m_x, m_y};
}
