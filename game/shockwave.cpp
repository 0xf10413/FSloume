/*
 * shockwave.cpp
 * Copyright (C) 2018 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "shockwave.h"
#include "rc_manager.h"
#include "config.h"
#include <iostream>


ShockWave::ShockWave (bool goingRight) : m_going_right(goingRight)
{
  m_texture = ResourceManager::getTexture("rShockwave");
  m_sprite.setTexture(*m_texture);
  m_vx = CG::SHOCKWAVE_SPEED;
}

void ShockWave::updateSprite()
{
  m_sprite.setPosition(
      m_x-CG::SHOCKWAVE_WIDTH/2,
      m_y-CG::SHOCKWAVE_HEIGHT
      );
}

void ShockWave::setPosition (float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

sf::Vector2f ShockWave::getPosition () const
{
  return {m_x, m_y};
}

void ShockWave::draw(sf::RenderWindow &w) const
{
  w.draw(m_sprite);
}

void ShockWave::move (float dt)
{
  m_x += m_vx*dt;
  m_y += m_vy*dt;
  updateSprite();
}