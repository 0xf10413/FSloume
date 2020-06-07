#ifndef F_NET_H
#define F_NET_H

#include <SFML/Graphics.hpp>
#include "config.h"
#include "tools/unique_drawable.h"

class Net : public MovingEntity
{
private:
  virtual void updateSprite() override;
public:
  Net ();
  void setX (float x);
  void setY (float y);
  void setPosition (float x, float y);
  sf::FloatRect getBox() const;
  sf::Vector2f getPosition() const;
};

#endif
