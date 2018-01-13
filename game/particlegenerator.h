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

struct Particle
{
  sf::Vector2f m_speed;
  sf::Time m_lifetime;
  void reset(std::mt19937 &mt);
  Particle() : m_speed(), m_lifetime(sf::Time::Zero) {};
};

class ParticleGenerator : public sf::Drawable, public sf::Transformable
{
private:
  std::vector<Particle> m_particles;
  int m_N; // On trace des N-gones
  sf::VertexArray m_vertices;

  sf::Vector2f m_emitpt;
  std::mt19937 m_mt;

  enum class Status {STARTED, PULSING, STOPPING, STOPPED} m_status;

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
  ParticleGenerator(size_t nb_part, int n);
  void setPosition(float x, float y);
  void animate(float dt);

  void start(bool pulse = false); // générer une salve de particules ?
  void stop(bool full_stop=false); // stoper brutalement ou progressivement ?
};

#endif /* !PARTICLEGENERATOR_H */
