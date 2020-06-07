/*
 * cloud.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "entities/cloudzone.h"
#include "config.h"
#include <random>
#include <sstream>


Cloud::Cloud(const std::string &tex_name) : m_snow(nullptr), m_rain(nullptr)
{
  m_texture = ResourceManager::getTexture(tex_name);
  m_sprite.setTexture(*m_texture);
  m_sprite.setScale((float)CG::CLOUD_WIDTH/(*m_texture).getSize().x,
      (float)CG::CLOUD_HEIGHT/(*m_texture).getSize().y);
}

void Cloud::updateSprite()
{
  m_sprite.setPosition(m_x - CG::CLOUD_WIDTH/2, m_y - CG::CLOUD_HEIGHT/2);
}

void Cloud::addSnow()
{
  if (!m_snow)
    m_snow.reset(new ParticleGenerator(30, "snowflake"));
  m_snow->start();
}

void Cloud::addRain()
{
  if (!m_rain)
    m_rain.reset(new ParticleGenerator(30, "raindrop"));
  m_rain->start();
}

void Cloud::rmRain()
{
  if (m_rain)
    m_rain->stop();
}

void Cloud::rmSnow()
{
  if (m_snow)
    m_snow->stop();
}

void Cloud::animate(float dt)
{
  m_x += m_vx * dt;
  m_y += m_vy * dt;
  if (m_snow)
  {
    m_snow->setPosition(m_x, m_y);
    m_snow->animate(dt/4);
  }
  else if (m_rain)
  {
    m_rain->setPosition(m_x, m_y);
    m_rain->animate(dt);
  }
  updateSprite();
}

void Cloud::setSpeed(float vx, float vy)
{
  m_vx = vx;
  m_vy = vy;
}

void Cloud::draw(sf::RenderTexture &w) const
{
  if (m_snow)
    w.draw(*m_snow);
  if (m_rain)
    w.draw(*m_rain);
  w.draw(m_sprite);
}

void Cloud::setPosition (float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

CloudZone::CloudZone(sf::FloatRect zone, size_t how_many) : m_zone(zone),
  m_clouds()
{
  m_clouds.reserve(how_many);

  std::random_device rd;
  std::mt19937 twister (rd());
  std::uniform_int_distribution<int> dist_rc(1,2);

  for (size_t i = 0; i < how_many; ++i)
  {
    std::ostringstream oss;
    oss << "rc_cloud" << dist_rc(twister) << "_png";
    m_clouds.push_back({oss.str()});
  }


  std::uniform_real_distribution<float> dist_x(m_zone.left-CG::CLOUD_WIDTH,
      m_zone.width+CG::CLOUD_WIDTH);
  std::uniform_real_distribution<float> dist_y(m_zone.top, m_zone.top + m_zone.height);
  std::uniform_real_distribution<float> dist_vx(CG::CLOUD_MIN_SPEED_X, CG::CLOUD_MAX_SPEED_X);
  std::uniform_real_distribution<float> dist_vy(CG::CLOUD_MIN_SPEED_Y, CG::CLOUD_MAX_SPEED_Y);

  for (Cloud &c : m_clouds)
  {
    c.setPosition(dist_x(twister), dist_y(twister));
    c.setSpeed(dist_vx(twister), dist_vy(twister));
  }
}

void CloudZone::animate(float dt)
{
  static std::bernoulli_distribution add_snow(CG::CLOUD_PROBABILITY_SNOW);
  static std::bernoulli_distribution add_rain(CG::CLOUD_PROBABILITY_RAIN);
  std::mt19937 twister ((std::random_device()()));
  for (Cloud &c : m_clouds)
  {
    if (c.m_x - CG::CLOUD_WIDTH > m_zone.left + m_zone.width)
    {
      c.m_x = m_zone.left - CG::CLOUD_WIDTH;
      if (add_snow(twister))
      {
        c.rmRain();
        c.addSnow();
      }
      else if (add_rain(twister))
      {
        c.rmSnow();
        c.addRain();
      }
      else
      {
        c.rmSnow();
        c.rmRain();
      }
    }
    if (c.m_y - CG::CLOUD_HEIGHT > m_zone.top + m_zone.height ||
        c.m_y + CG::CLOUD_HEIGHT < m_zone.top)
      c.m_vy *= -1;
    c.animate(dt);
  }
}

void CloudZone::updateSprite()
{

}

void CloudZone::draw(sf::RenderTexture &w) const
{
  for (const Cloud &c : m_clouds)
    c.draw(w);

}
