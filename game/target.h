#ifndef TARGET_H
#define TARGET_H

#include "unique_drawable.h"

/* Une mire pour aider au debug sur android */
class Target : public MovingEntity
{
public:
  Target();
  virtual ~Target() = default;
  virtual void updateSprite() override;
  void setPosition(float x, float y);
};


#endif /* !TARGET_H */
