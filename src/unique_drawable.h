#ifndef UNIQUE_DRAWABLE_H
#define UNIQUE_DRAWABLE_H

#include <SFML/Graphics.hpp>

/**
 * Une classe fréquemment utilisée. Cumule un sprite, une texture et une image.
 * Pour que l'objet soit directement dessinable, il hérite de sf::Sprite
 */
class UniqueDrawable
{
protected:
  sf::Sprite m_sprite;
  sf::Image m_image;
  sf::Texture m_texture;
public:
  UniqueDrawable();
  virtual ~UniqueDrawable() = default;
  inline void setPosition(float x, float y)
  { m_sprite.setPosition(x, y); }

  inline sf::Vector2f getPosition()
  { return m_sprite.getPosition(); }

  virtual void draw(sf::RenderWindow &);
};

class MovingEntity : public UniqueDrawable
{
protected:
  /* Positions et vitesses du centre d'inertie */
  float m_x, m_y;
  float m_vx, m_vy;
public:
  MovingEntity();
  virtual ~MovingEntity()= default;
};

#endif /* !UNIQUE_DRAWABLE_H */
