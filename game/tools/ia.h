/*
 * tools/ia.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef IA_H
#define IA_H

#include "entities/slime.h"
#include "entities/shockwave.h"

/* Une IA à lier à un slime */
class IA
{
public:
  enum Difficulty {
    TOO_EASY, // only moves left and right
    EASY, // can jump
    NORMAL, // can double jump, dash, fast return, and will try to dodge shockwave
    HARD, // attacks
    };
private:
  Difficulty m_difficulty;
public:
  IA(Difficulty d);
  void interact (Slime &s, const Ball &b, const sf::Vector2f &dangerPt, const ShockWave &sw);
};


#endif /* !IA_H */
