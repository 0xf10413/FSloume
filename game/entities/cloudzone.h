/*
 * cloud.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CLOUD_H
#define CLOUD_H

#include "tools/unique_drawable.h"
#include "rc_manager.h"
#include "entities/particlegenerator.h"

class Cloud : public MovingEntity
{
private:
  virtual void updateSprite() override;
  friend class CloudZone;
  void setSpeed(float vx, float vy);
  std::unique_ptr<ParticleGenerator> m_snow, m_rain;
public:
  Cloud(const std::string &tex_name);
  void setPosition (float x, float y);
  virtual void draw(sf::RenderTexture &) const override;
  void animate (float dt);
  void addSnow();
  void addRain();
  void rmSnow();
  void rmRain();
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
  virtual void draw(sf::RenderTexture &w) const override;
};

#endif /* !CLOUD_H */
