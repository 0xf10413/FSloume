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

Particle::Particle(const std::string &tex_name) : MovingEntity(),
  m_age(-1.f)
{
  m_texture = ResourceManager::getTexture(tex_name);
  auto size = m_texture.lock()->getSize();
  m_sprite.setTexture(*m_texture.lock());
  m_sprite.setScale((float)CG::BALL_RADIUS/size.x, (float)CG::BALL_RADIUS/size.y);
  m_sprite.setOrigin(CG::BALL_RADIUS/2,CG::BALL_RADIUS/2);
}

void Particle::setPosition(float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

void Particle::draw (sf::RenderWindow &w) const
{
  if (m_age >= 0.)
    w.draw(m_sprite);
}

void Particle::updateSprite()
{
  m_sprite.setPosition(
      m_x, m_y
      );
}

void Particle::reset (sf::Vector2f pos, sf::Vector2f speed, float age)
{
  reinit();
  setPosition(pos.x,pos.y);
  m_vx = speed.x;
  m_vy = speed.y;
  m_age = age;
}

void Particle::animate(float dt)
{
  m_vy += CG::GRAVITY*dt;
  m_x += m_vx*dt;
  m_y += m_vy*dt;
  m_age -= dt;
  updateSprite();
}


/*******************/

ParticleGenerator::ParticleGenerator(const std::string &texname) :
  m_particles(200, texname), m_texname(texname), m_nb_alive(0), m_nb_alive_max(50),
  m_mt(std::random_device()())
{
  std::uniform_real_distribution<float> distr_age(0, 2);
  for (Particle &p : m_particles)
    p.m_age = distr_age(m_mt);
}

void ParticleGenerator::updateSprite()
{
}

void ParticleGenerator::animate(float dt)
{
  std::uniform_real_distribution<float> distr_x (-40, 40), distr_y (-100, -20);
  m_nb_alive = 0;
  for (Particle &p : m_particles)
  {
    if (!p.dead())
      ++m_nb_alive;
    else if (m_nb_alive < m_nb_alive_max)
    {
      p.reset({m_x, m_y}, {distr_x(m_mt), distr_y(m_mt)}, 2);
      ++m_nb_alive;
    }
    p.animate(dt);
  }
}

void ParticleGenerator::draw(sf::RenderWindow &w) const
{
  for (const Particle &p : m_particles)
    p.draw(w);
}

void ParticleGenerator::setPosition(float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}
