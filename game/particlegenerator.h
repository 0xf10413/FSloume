/*
 * particlegenerator.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include "unique_drawable.h"
#include <random>

class Particle : public MovingEntity
{
private:
  float m_age;
  virtual void updateSprite() override;
  friend class ParticleGenerator;

public:
  Particle(const std::string &tex_name);

  inline bool dead() { return m_age < 0.f; }
  void reset (sf::Vector2f pos, sf::Vector2f speed, float age);

  void setPosition (float x, float y);
  virtual void draw (sf::RenderWindow &) const override;
  void animate (float dt);
};

class ParticleGenerator : public MovingEntity
{
private:
  std::vector<Particle> m_particles;
  std::string m_texname;
  size_t m_nb_alive, m_nb_alive_max;
  std::mt19937 m_mt;
public:
  ParticleGenerator(const std::string &textname);
  void setPosition(float x, float y);
  virtual void updateSprite() override;
  void animate(float dt);
  virtual void draw(sf::RenderWindow &w) const override;
};

#endif /* !PARTICLEGENERATOR_H */
