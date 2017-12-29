/*
 * dangerpt.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "dangerpt.h"
#include "config.h"
#include "rc_manager.h"


DangerPoint::DangerPoint() : MovingEntity()
{
  m_texture = ResourceManager::getTexture("danger_pt");
  m_sprite.setTexture(*m_texture.lock());
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
