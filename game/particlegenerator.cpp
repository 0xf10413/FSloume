/*
 * particlegenerator.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "particlegenerator.h"
#include "rc_manager.h"
#include "config.h"
#include <iostream>
#include <complex>


/*******************/

ParticleGenerator::ParticleGenerator(size_t nb_part, int N) :
  m_particles(nb_part),
  m_N (N),
  m_vertices(sf::Lines, 2*nb_part*m_N),
  m_emitpt(),
  m_mt(std::random_device()()),
  m_status(Status::STOPPED)
{
  for (size_t i = 0; i < nb_part; ++i)
    for (int j = 0; j < 2*m_N; ++j)
      m_vertices[2*m_N*i+j].color = sf::Color(j*255/(2*m_N), 255, 255);
}

void Particle::reset(std::mt19937 &mt)
{
  static std::uniform_real_distribution<float> distr_vx (-20, 20);
  static std::uniform_real_distribution<float> distr_vy (-20, 20);
  static std::uniform_real_distribution<float> distr_lifetime (-.1, 4);

  m_speed = {distr_vx(mt), distr_vy(mt)};
  m_lifetime = sf::seconds(distr_lifetime(mt));
}

/* Dessine un N-gone de taille radius dans le vertex array v, à l'indice index */
static inline void drawNGon(sf::VertexArray &v, size_t index,
    sf::Vector2f at, int N, float radius)
{
  const std::complex<float> j (0,1);
  std::complex<float> prev_pt = radius, next_pt;
  for (int k = 0; k < N; ++k)
  {
    next_pt = radius*std::exp(2*(k+1)*(float)M_PI/N*j);
    v[index+2*k].position = at + sf::Vector2f(prev_pt.real(), prev_pt.imag());
    v[index+2*k+1].position = at + sf::Vector2f{next_pt.real(), next_pt.imag()};
    prev_pt = next_pt;
  }
}

void ParticleGenerator::animate(float dt)
{
  for (size_t i = 0; i < m_particles.size(); ++i)
  {
    if (m_particles[i].m_lifetime <= sf::Time::Zero || m_status == Status::STOPPED)
    {
      if (m_status == Status::STARTED || m_status == Status::PULSING)
      {
        m_particles[i].reset(m_mt);
        drawNGon(m_vertices, 2*m_N*i, m_emitpt, m_N, CG::PARTICLE_LENGTH);
        for (int j = 0; j < 2*m_N; ++j)
          m_vertices[2*m_N*i+j].color.a = 255;
      }
      else
        for (int j = 0; j < 2*m_N; ++j)
          m_vertices[2*m_N*i+j].color.a = 0;
    }
    for (int j = 0; j < 2*m_N; ++j)
      m_vertices[2*m_N*i+j].position += m_particles[i].m_speed*dt;
    m_particles[i].m_lifetime -= sf::seconds(dt);
    //m_particles[i].m_speed.y += CG::GRAVITY*dt;
  }

  if (m_status == Status::PULSING)
    m_status = Status::STOPPING;
}

void ParticleGenerator::setPosition(float x, float y)
{
  m_emitpt = {x,y};
}

void ParticleGenerator::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
  if (m_status == Status::STOPPED)
    return;
  states.transform *= getTransform();
  states.texture = nullptr;
  target.draw(m_vertices, states);
}

void ParticleGenerator::start(bool pulse)
{
  m_status = pulse ? Status::PULSING : Status::STARTED;
}

void ParticleGenerator::stop(bool full_stop)
{
  m_status = full_stop ? Status::STOPPED : Status::STOPPING;
}
