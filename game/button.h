#ifndef F_BUTTON_H
#define F_BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include "unique_drawable.h"

class Button : public MovingEntity
{
private:
  const sf::Font &m_font;
  sf::Text m_text;
  sf::Color m_color;

  sf::Vector2f m_size;

  virtual void updateSprite() override;
public:
  Button(const sf::Font &font, const std::string &text, sf::Color color);
  void setPosition(float x, float y);
  std::string getTextString() const;
  virtual void draw(sf::RenderWindow &) const override;
  bool wasIClicked(const sf::Event &event) const;
  sf::Vector2f getSize() const;
};


#endif /* !F_BUTTON_H */
