#ifndef F_BALL_H
#define F_BALL_H

#include <SFML/Graphics.hpp>
#include "unique_drawable.h"
#include "main.h"

class Ball : public MovingEntity
{
private:
  virtual void updateSprite() override;
  sf::FloatRect m_clamp;
public:
	Ball ();
  void setX (float x);
  void setY (float y);
  void clampTo (const sf::FloatRect &clamp);
  void move (float dt);

  sf::Vector2f getPosition() const;
  sf::Vector2f getSpeed() const;
};

#endif
