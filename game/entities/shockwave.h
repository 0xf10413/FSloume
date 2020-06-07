/*
 * entities/shockwave.h
 * Copyright (C) 2018 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SHOCKWAVE_H
#define SHOCKWAVE_H

#include <SFML/Graphics.hpp>
#include "tools/unique_drawable.h"
#include "entities/particlegenerator.h"

class ShockWave : public MovingEntity
{
private:
  bool m_going_right;
  virtual void updateSprite() override;
public:
  ShockWave (bool goingRight);
  void setX (float x);
  void setY (float y);
  void setPosition (float x, float y);
  void move (float dt);

  sf::Vector2f getPosition() const;
  virtual void draw(sf::RenderTexture &w) const override;
};

#endif /* !SHOCKWAVE_H */
