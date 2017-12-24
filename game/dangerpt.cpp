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
  m_image.create(CG::DANGERPT_WIDTH, CG::DANGERPT_WIDTH, m_color);
  for (int i = CG::DANGERPT_WIDTH/10; i < 7*CG::DANGERPT_WIDTH/10; ++i)
    for (int j = CG::DANGERPT_WIDTH/2-1; j < CG::DANGERPT_WIDTH/2+2; ++j)
      m_image.setPixel(j, i, sf::Color::Black);
  for (int i = CG::DANGERPT_WIDTH/2-2; i < CG::DANGERPT_WIDTH/2+3; ++i)
    for (int j = 9*CG::DANGERPT_WIDTH/10-2; j < 9*CG::DANGERPT_WIDTH/10+2; ++j)
      m_image.setPixel(i, j, sf::Color::Black);
  for (int i = 0; i < CG::DANGERPT_WIDTH; ++i)
    for (int j = 0; j < CG::DANGERPT_WIDTH; ++j)
    {
      if (-2*i + CG::DANGERPT_WIDTH > j ||
          2*i - CG::DANGERPT_WIDTH > j)
        m_image.setPixel(i,j,sf::Color(0,0,0,0));
    }
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void DangerPoint::updateSprite()
{
  m_sprite.setPosition(
      m_x - CG::BALL_RADIUS,
      m_y - CG::BALL_RADIUS
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
