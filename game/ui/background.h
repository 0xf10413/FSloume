/*
 * background.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML/Graphics.hpp>
#include "tools/unique_drawable.h"
#include "entities/cloudzone.h"
#include "entities/particlegenerator.h"

class Background : public MovingEntity
{
private:
  CloudZone m_clouds;
public:
  Background();
  void animate(float dt);
  virtual void updateSprite() override;
  virtual void draw(sf::RenderTexture &w) const override;
};

#endif /* !BACKGROUND_H */
