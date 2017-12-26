/*
 * background.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "background.h"
#include "config.h"

Background::Background() :
  m_clouds(sf::FloatRect(0,CG::HEIGHT/5, CG::WIDTH, CG::HEIGHT/5), CG::NB_MAX_CLOUDS)
{
  m_image.create(CG::WIDTH, CG::HEIGHT, sf::Color::White);
  for (int i = 0; i < CG::WIDTH; ++i)
    for (int j = 0; j < CG::HEIGHT; ++j)
      m_image.setPixel(i, j, sf::Color(0, j*100/CG::HEIGHT, j*255/CG::HEIGHT));
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
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

void Background::draw(sf::RenderWindow &w) const
{
  w.draw(m_sprite);
  m_clouds.draw(w);
}
