/*
 * background.cpp
 * Copyright (C) 2017 flo <flo@knightknight>
 *
 * Distributed under terms of the MIT license.
 */

#include "background.h"
#include "config.h"

Background::Background()
{
  m_image.create(CG::WIDTH, CG::HEIGHT, sf::Color(0, 127, 255));
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void Background::updateSprite()
{
  m_sprite.setPosition(
      m_x - (float)CG::WIDTH,
      m_y - (float)CG::HEIGHT
      );
}
