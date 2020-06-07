#ifndef LINE_H
#define LINE_H

#include <SFML/Graphics.hpp>
#include "tools/unique_drawable.h"

class Path : public UniqueDrawable
{
private:
  sf::VertexArray m_pieces;
  sf::Color m_color;
public:
  Path(int length, sf::Color color);
  virtual ~Path() = default;
  virtual void draw(sf::RenderWindow &) const;
  void setPosition(int i, float x, float y);
  bool traverseGame() const;
};

#endif /* !LINE_H */
