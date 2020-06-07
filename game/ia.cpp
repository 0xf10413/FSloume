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

void IA::interact (Slime &s, const Ball &b, const sf::Vector2f &dangerPt,
    const ShockWave &sw)
{
  // Jump to send back the ball
  if (m_difficulty >= EASY &&
      std::abs(b.getPosition().x - s.m_x) < CG::SLIME_HEIGHT &&
      std::abs(b.getPosition().y - s.m_y) < CG::JUMP_MAX_HEIGHT + CG::SLIME_HEIGHT)
    if (!s.jump() && m_difficulty >= NORMAL)
      s.doubleJump();

  bool avoid_shockwave =
    m_difficulty >= EASY
    && std::abs(sw.getPosition().x - s.m_x) < 2*CG::SLIME_WIDTH
    && (s.m_alignLeft ? sw.getPosition().x > s.m_x : sw.getPosition().x < s.m_x)
    && s.m_vy >= 0;
  if (avoid_shockwave)
    if (!s.jump() && m_difficulty >= NORMAL)
      s.doubleJump();

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
      if (s.m_onGround && m_difficulty >= NORMAL)
        s.m_vx *= 2;
      else if (!avoid_shockwave && m_difficulty >= NORMAL)
        s.antijump();
    }
  }
  else /* no danger right now */
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

    /* begin a ground pound */
    if (m_difficulty >= HARD
          && !b.traverseGame() && (s.m_alignLeft ? b.getPosition().x > CG::WIDTH/2
          : b.getPosition().x < CG::WIDTH/2))
    {
      switch(s.m_movingv_status)
      {
        case Slime::MovingVStatus::STOPPED:
          s.jump();
          break;
        case Slime::MovingVStatus::JUMPING:
          break;
        case Slime::MovingVStatus::JUMPING_WAIT:
          s.doubleJump();
          break;
        case Slime::MovingVStatus::DOUBLE_JUMPING:
          s.m_apec = s.m_y;
          if (s.canGroundPound(true))
            s.antijump();
          break;
        case Slime::MovingVStatus::FAST_LAND:
          s.m_apec = s.m_y;
          s.groundPound();
          break;
        case Slime::MovingVStatus::GROUND_POUND:
          break;
        case Slime::MovingVStatus::FORCE_JUMPING:
          break;
      }
    }
  }

  if (s.m_movingv_status == Slime::MovingVStatus::GROUND_POUND)
  {
    float mid_height = .33*CG::HEIGHT + .66*s.m_apec;
    if (s.m_y < s.m_apec)
      s.setTheta(180*(s.m_y-s.m_apec)/(CG::HEIGHT-s.m_apec) *(s.m_alignLeft ? 1 : -1));
    else if (s.m_y < mid_height)
      s.setTheta(360*(s.m_y-mid_height)/(mid_height-s.m_apec) *(s.m_alignLeft ? 1 : -1));
    else
    {
      s.setTheta(0);
      s.antijump();
    }
  }
}
