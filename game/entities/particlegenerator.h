/*
 * entities/particlegenerator.h
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef PARTICLEGENERATOR_H
#define PARTICLEGENERATOR_H

#include "tools/unique_drawable.h"
#include <random>

struct Particle
{
  sf::Vector2f m_speed;
  sf::Time m_lifetime;
  void reset(std::mt19937 &mt);
  void reset(std::mt19937 &mt, sf::Vector2f dir);
  Particle() : m_speed(), m_lifetime(sf::Time::Zero) {};
};

class ParticleGenerator : public sf::Drawable, public sf::Transformable
{
private:
  std::vector<Particle> m_particles;
  sf::VertexArray m_vertices;
  std::shared_ptr<sf::Texture> m_texture;

  sf::Vector2f m_emitpt;
  std::mt19937 m_mt;

  enum class Status {STARTED, PULSING, STOPPING, STOPPED} m_status;

  virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
public:
  ParticleGenerator(size_t nb_part, const std::string &tex_name);
  void setPosition(float x, float y);
  void animate(float dt);

  void start();
  void pulse(sf::Vector2f dir, sf::Color); // générer une salve de particules dans une direction
  void stop(bool full_stop=false); // stoper brutalement ou progressivement ?
};

#endif /* !PARTICLEGENERATOR_H */
