/*
 * background.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "background.h"
#include "rc_manager.h"
#include "config.h"

Background::Background() :
  m_clouds(sf::FloatRect(0,CG::HEIGHT/5, CG::WIDTH, CG::HEIGHT/5), CG::NB_MAX_CLOUDS)
{
  m_texture = ResourceManager::getTexture("background");
  m_sprite.setTexture(*m_texture);
}

void Background::animate(float dt)
{
  m_clouds.animate(dt);
}

void Background::updateSprite()
{
  m_sprite.setPosition(
      m_x - (float)CG::WIDTH,
      m_y - (float)CG::HEIGHT
      );
}

void Background::draw(sf::RenderTexture &w) const
{
  w.draw(m_sprite);
  m_clouds.draw(w);
}
