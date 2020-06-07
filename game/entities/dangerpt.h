/*
 * dangerpt.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef DANGERPT_H
#define DANGERPT_H

#include "tools/unique_drawable.h"

class DangerPoint : public MovingEntity
{
public:
  DangerPoint();
  virtual void updateSprite();
  void setPosition(const sf::Vector2f &v);
  sf::Vector2f getPosition();
};

#endif /* !DANGERPT_H */
