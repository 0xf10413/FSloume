#ifndef COUNTER_H
#define COUNTER_H

#include "unique_drawable.h"

class Counter : public MovingEntity
{
private:
  size_t m_count;
  bool m_alignLeft;
  const sf::Font &m_font;
  sf::Text m_text;

  virtual void updateSprite() override;
public:
  Counter(size_t init, bool alignLeft, const sf::Font &font);
  Counter (const Counter &) = delete;
  Counter &operator =(const Counter &) = delete;


  void setValue(size_t value);

  void setX(float x);
  void setY(float y);
  void setPosition(float x, float y);
  Counter &operator++();
  Counter &operator--();
  virtual void draw (sf::RenderWindow &w) const override;
  virtual void reinit() override;
};

#endif /* !COUNTER_H */
