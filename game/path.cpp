#include "path.h"
#include "config.h"

Path::Path(int length, sf::Color color) :
  UniqueDrawable(),
  m_pieces(sf::Lines, 2*length+1),
  m_color(color)
{
  size_t n = m_pieces.getVertexCount();
  for (size_t i = 0; i < n; ++i)
  {
    m_pieces[i].color = sf::Color(
        (sf::Uint8)(m_color.r),
        (sf::Uint8)(m_color.g),
        (sf::Uint8)(m_color.b),
        (sf::Uint8)(255-255*i/n));
    m_pieces[i].position = sf::Vector2f((float)i,(float)i);
  }
}

void Path::draw(sf::RenderWindow &w) const
{
  w.draw(m_pieces);
}

void Path::setPosition(int i, float x, float y)
{
  if (i == 0)
    m_pieces[0].position = {x, y};

  m_pieces[2*i+1].position = {x, y};
  m_pieces[2*i+1].color.r = (float)((x > CG::WIDTH/2)*255);
  m_pieces[2*i+1].color.b = 255;
  m_pieces[2*i+1].color.g = (unsigned)(x < CG::WIDTH/2)*255;
  m_pieces[2*i+2].position = {x, y};
  m_pieces[2*i+2].color.r = (x > CG::WIDTH/2)*255;
  m_pieces[2*i+2].color.b = 255;
  m_pieces[2*i+2].color.g = (x < CG::WIDTH/2)*255;
}
