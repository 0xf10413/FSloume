/*
 * ia.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "ia.h"


IA::IA (Difficulty d) : m_difficulty(d)
{
}

void IA::interact (Slime &s, const Ball &b, const sf::Vector2f &dangerPt)
{
  if (std::abs(b.getPosition().x - s.m_x) < SLIME_HEIGHT &&
      std::abs(b.getPosition().y - s.m_y) < JUMP_MAX_HEIGHT + SLIME_HEIGHT)
    s.jump();
  if (dangerPt.x >= 0)
  {
    if (s.m_x < dangerPt.x)
      s.m_vx = SLIME_HORIZONTAL_SPEED;
    else
      s.m_vx = -SLIME_HORIZONTAL_SPEED;

  }
  else
    if (std::abs(s.m_x - b.getPosition().x) < SLIME_WIDTH/5)
      s.m_vx = SLIME_HORIZONTAL_SPEED;
}
