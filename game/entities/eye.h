#ifndef EYE_H
#define EYE_H

#include "tools/unique_drawable.h"

class Eye : public MovingEntity
{
private:
  virtual void updateSprite() override;
public:
  Eye(bool alignLeft);
  void setPosition(float x, float y);
  void lookAt(const sf::Vector2f &at);
};

#endif /* !EYE_H */
