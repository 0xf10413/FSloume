/*
 * background.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>
#include "unique_drawable.h"

class Background : public MovingEntity
{
private:
public:
  Background();
  virtual void updateSprite();
};

#endif /* !BACKGROUND_H */
