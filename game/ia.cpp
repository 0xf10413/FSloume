/*
 * ia.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "ia.h"
#include "config.h"


IA::IA (Difficulty d) : m_difficulty(d)
{
}

void IA::interact (Slime &s, const Ball &b, const sf::Vector2f &dangerPt)
{
  if (std::abs(b.getPosition().x - s.m_x) < CG::SLIME_HEIGHT &&
      std::abs(b.getPosition().y - s.m_y) < CG::JUMP_MAX_HEIGHT + CG::SLIME_HEIGHT)
    s.jump();

  if (s.lockedRetreat())
    return;

  s.m_vx = 0;

  if (dangerPt.x >= 0 /*&& (std::abs(dangerPt.x - s.m_x) > 4)*/
      && ((s.m_alignLeft && dangerPt.x <= CG::WIDTH/2) ||
        (!s.m_alignLeft && dangerPt.x >= CG::WIDTH/2)))
  {
    if (s.m_x < dangerPt.x)
      s.m_vx = CG::SLIME_HORIZONTAL_SPEED;
    else
      s.m_vx = -CG::SLIME_HORIZONTAL_SPEED;
    if (std::abs(s.m_x -dangerPt.x) > CG::SLIME_WIDTH)
    {
      if (s.m_onGround)
        s.m_vx *= 2;
      else
        s.antijump();
    }
  }
  else
  {
    if (std::abs(s.m_x - b.getPosition().x) < CG::SLIME_WIDTH/5)
      s.m_vx = (s.m_alignLeft ? -1 : 1) * CG::SLIME_HORIZONTAL_SPEED;
    if (!s.m_alignLeft && b.getPosition().x < CG::WIDTH/2) // Replacement au centre
    {
      if (s.m_x < 3*CG::WIDTH/4)
        s.m_vx = CG::SLIME_HORIZONTAL_SPEED;
      else if (s.m_x - 3*CG::WIDTH/4 > 4) // Tremblements
        s.m_vx = -CG::SLIME_HORIZONTAL_SPEED;
    }
    else if (s.m_alignLeft && (b.getPosition().x > CG::WIDTH/2))
    {
      if (s.m_x < CG::WIDTH/4)
        s.m_vx = CG::SLIME_HORIZONTAL_SPEED;
      else if (s.m_x - CG::WIDTH/4 > 4) // Tremblements
        s.m_vx = -CG::SLIME_HORIZONTAL_SPEED;
    }
  }
}
