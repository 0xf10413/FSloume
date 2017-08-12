#ifndef F_MENU_H
#define F_MENU_H

#include <vector>
#include "button.h"
#include "unique_drawable.h"

class Menu : public MovingEntity
{
private:
  const sf::Font &m_font;
  std::vector<Button*> m_buttons;
  sf::Vector2f m_size;
  std::vector<sf::Vector2f> m_sizes;

  virtual void updateSprite() override;
  void rebuildMenu(); // Refaire les données graphiques, suite à l'ajout d'un bouton
public:
  Menu(const sf::Font &font);
  ~Menu();
  void addButton (const std::string &text, sf::Color color);
  void setPosition(float x, float y);
  virtual void draw(sf::RenderWindow &) const override;
  std::string wasIClicked(const sf::Event &event) const;
};

#endif /* !F_MENU_H */
