#ifndef TARGET_H
#define TARGET_H

#include "tools/unique_drawable.h"

/* Une mire pour aider au debug sur android */
class Target : public MovingEntity
{
private:
  bool m_visible;
public:
  Target();
  virtual ~Target() = default;
  virtual void updateSprite() override;
  virtual void draw(sf::RenderWindow &w) const override;
  void hide();
  void setPosition(float x, float y);
};


#endif /* !TARGET_H */
