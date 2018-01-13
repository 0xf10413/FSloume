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

ParticleGenerator::ParticleGenerator(size_t nb_part, const std::string &tex_name) :
  m_particles(nb_part),
  m_vertices(sf::Triangles, 6*nb_part),
  m_texture(),
  m_emitpt(),
  m_mt(std::random_device()()),
  m_status(Status::STOPPED)
{
  m_texture = ResourceManager::getTexture(tex_name);
  sf::Vector2u size = m_texture->getSize();
  for (size_t i = 0; i < nb_part; ++i)
  {
    sf::Vertex *triangles = &m_vertices[6*i];

    /* Lower triangle */
    triangles[0].texCoords = sf::Vector2f(0, size.y);
    triangles[1].texCoords = sf::Vector2f(size.x, size.y);
    triangles[2].texCoords = sf::Vector2f(0, 0);

    /* Upper triangle */
    triangles[3].texCoords = sf::Vector2f(size.x, size.y);
    triangles[4].texCoords = sf::Vector2f(size.x, 0);
    triangles[5].texCoords = sf::Vector2f(0, 0);
  }
}

/* Dessine le carré de coté length centré en at, en s'appuyant sur le vertexarray v */
static inline void drawSquareTriangle(sf::Vertex *v, sf::Vector2f at, int length)
{
  v[0].position = at + sf::Vector2f(-length/2, length/2);
  v[1].position = at + sf::Vector2f(length/2, length/2);
  v[2].position = at + sf::Vector2f(-length/2, -length/2);
  v[3].position = at + sf::Vector2f(length/2, length/2);
  v[4].position = at + sf::Vector2f(length/2, -length/2);
  v[5].position = at + sf::Vector2f(-length/2, -length/2);
}


void Particle::reset(std::mt19937 &mt)
{
  static std::uniform_real_distribution<float> distr_vx (-40, 40);
  static std::uniform_real_distribution<float> distr_vy (-40, 40);
  static std::uniform_real_distribution<float> distr_lifetime (-.1, 2);

  m_speed = {distr_vx(mt), distr_vy(mt)};
  m_lifetime = sf::seconds(distr_lifetime(mt));
}

void Particle::reset(std::mt19937 &mt, sf::Vector2f dir)
{
  static std::uniform_real_distribution<float> distr_v (1, 2);
  static std::uniform_real_distribution<float> distr_theta (-M_PI/2, M_PI/2);
  static std::uniform_real_distribution<float> distr_lifetime (-.1, .5);

  sf::Vector2f speed = distr_v(mt)*dir;
  float theta = distr_theta(mt);
  m_speed = sf::Vector2f(speed.x*cos(theta) - speed.y*sin(theta),
      speed.x*sin(theta) + speed.y*cos(theta));

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
        drawSquareTriangle(&m_vertices[6*i], m_emitpt, CG::PARTICLE_LENGTH);
        for (int j = 0; j < 6; ++j)
          m_vertices[6*i+j].color.a = 255;
      }
      else
        for (int j = 0; j < 6; ++j)
          m_vertices[6*i+j].color.a = 0;

    }
    for (int j = 0; j < 6; ++j)
      m_vertices[6*i+j].position += m_particles[i].m_speed*dt;
    m_particles[i].m_lifetime -= sf::seconds(dt);
    m_particles[i].m_speed.y += CG::GRAVITY*dt;
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
  states.texture = m_texture.get();
  target.draw(m_vertices, states);
}

void ParticleGenerator::start()
{
  m_status = Status::STARTED;
}

void ParticleGenerator::pulse(sf::Vector2f dir, sf::Color color)
{
  m_status = Status::PULSING;
  for (size_t i = 0; i < m_particles.size(); ++i)
  {
    if (m_particles[i].m_lifetime <= sf::Time::Zero)
    {
      m_particles[i].reset(m_mt, dir);
      drawSquareTriangle(&m_vertices[6*i], m_emitpt, CG::PARTICLE_LENGTH);
      for (int j = 0; j < 6; ++j)
      {
        m_vertices[6*i+j].color = color;
        m_vertices[6*i+j].color.a = 255;
      }
    }
  }
  m_status = Status::STOPPING;
}

void ParticleGenerator::stop(bool full_stop)
{
  m_status = full_stop ? Status::STOPPED : Status::STOPPING;
}
