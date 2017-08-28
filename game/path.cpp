#include "path.h"

Path::Path(int length, sf::Color color) :
  UniqueDrawable(),
  m_pieces(sf::Lines, length),
  m_color(color)
{
  size_t n = m_pieces.getVertexCount();
  for (size_t i = 0; i < n; ++i)
  {
    m_pieces[i].color = m_color;
    m_pieces[i].position = sf::Vector2f((float)i,(float)i);
  }
}

void Path::draw(sf::RenderWindow &w) const
{
  w.draw(m_pieces);
}

void Path::setPosition(int i, float x, float y)
{
  m_pieces[i].position = {x, y};
}
