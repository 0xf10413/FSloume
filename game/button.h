#ifndef F_BUTTON_H
#define F_BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>
#include "unique_drawable.h"
#include "utils.h"

class Button : public HeavyMovingEntity
{
private:
  const sf::Font &m_font;
  sf::Text m_text;
  sf::Color m_color;

  sf::Vector2f m_textSize;
  sf::Vector2f m_margin; // Pas de différence entre left/right et bot/top
  sf::Vector2f m_padding; // Pas de différence entre left/right et bot/top

  virtual void updateSprite() override;
public:
  Button(const sf::Font &font, const std::string &text, sf::Color color,
      sf::Vector2f margin={}, sf::Vector2f padding={});
  virtual ~Button() = default;
  virtual void draw(sf::RenderWindow &) const override;

  void setPosition(float x, float y);
  std::string getTextString() const;
  bool wasIClicked(const sf::Event &event) const;
  sf::Vector2f getSize() const; // Donne la taille hors-margin
  sf::Vector2f getTotalSize() const;
};


#endif /* !F_BUTTON_H */
