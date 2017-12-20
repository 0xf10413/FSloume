/*
 * ia.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef IA_H
#define IA_H

#include "slime.h"

/* Une IA à lier à un slime */
class IA
{
public:
  enum class Difficulty {TOO_EASY};
private:
  Difficulty m_difficulty;
public:
  IA(Difficulty d);
  void interact (Slime &s, const Ball &b, const sf::Vector2f &dangerPt);
};


#endif /* !IA_H */
