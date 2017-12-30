/*
 * cloud.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CLOUD_H
#define CLOUD_H

#include "unique_drawable.h"
#include "rc_manager.h"

class Cloud : public MovingEntity
{
private:
  virtual void updateSprite() override;
  friend class CloudZone;
  void setSpeed(float vx, float vy);
public:
  Cloud(const std::string &tex_name);
  void setPosition (float x, float y);
  virtual void draw (sf::RenderWindow &) const override;
  void animate (float dt);
};

class CloudZone : public MovingEntity
{
private:
  sf::FloatRect m_zone;
  std::vector<Cloud> m_clouds;
public:
  CloudZone (sf::FloatRect zone, size_t how_many);
  virtual void updateSprite() override;
  void animate(float dt);
  virtual void draw(sf::RenderWindow &w) const override;
};

#endif /* !CLOUD_H */
