#include "target.h"
#include "config.h"
#include "rc_manager.h"

Target::Target() : m_visible(false)
{
  m_texture = ResourceManager::getTexture("target");
  m_sprite.setTexture(*m_texture);
}

void Target::updateSprite()
{
  m_visible = true;
  m_sprite.setPosition(
      m_x-(float)CG::TARGET_WIDTH/2,
      m_y-(float)CG::TARGET_HEIGHT/2
      );
}

void Target::hide()
{
  m_visible = false;
}

void Target::setPosition(float x, float y)
{
  m_x = x; m_y = y;
  updateSprite();
}

void Target::draw(sf::RenderTexture &w) const
{
  if (!m_visible)
    return;
  UniqueDrawable::draw(w);
}
