#ifndef F_BALL_H
#define F_BALL_H

#include <SFML/Graphics.hpp>
#include "unique_drawable.h"
#include "path.h"
#include "config.h"
#include "particlegenerator.h"

class Ball : public MovingEntity
{
private:
  virtual void updateSprite() override;
  sf::FloatRect m_clamp;
  Path m_path;
public:
	Ball ();
  void setX (float x);
  void setY (float y);
  void clampTo (const sf::FloatRect &clamp);
  void move (float dt);

  sf::Vector2f getPosition() const;
  sf::Vector2f getSpeed() const;
  void setSpeed(const sf::Vector2f &);
  virtual void draw(sf::RenderWindow &w) const override;

  void updatePath(int i);
};

#endif
