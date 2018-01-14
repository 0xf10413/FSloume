#include "net.h"
#include "rc_manager.h"

Net::Net () : MovingEntity()
{
  m_texture = ResourceManager::getTexture("net");
  m_sprite.setTexture(*m_texture);
}

void Net::setX (float x)
{
  m_x = x;
  updateSprite();
}

void Net::setY (float y)
{
  m_y = y;
  updateSprite();
}

void Net::setPosition (float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

sf::Vector2f Net::getPosition() const
{
  return {m_x, m_y};
}

void Net::updateSprite()
{
  m_sprite.setPosition(
      m_x - (float)CG::NET_WIDTH/2,
      m_y - (float)CG::NET_HEIGHT/2
      );
}

sf::FloatRect Net::getBox() const
{
  sf::FloatRect rect;
  rect.top = m_y - (float)CG::NET_HEIGHT/2;
  rect.left = m_x - (float)CG::NET_WIDTH/2;
  rect.height = (float)CG::NET_HEIGHT;
  rect.width = (float)CG::NET_WIDTH;
  return rect;
}
