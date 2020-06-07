#ifndef F_MENU_H
#define F_MENU_H

#include <vector>
#include "button.h"
#include "tools/unique_drawable.h"

/* TODO:
 * ajouter du border
 * ajouter du border-radius
 */

class Menu : public HeavyMovingEntity
{
private:
  const sf::Font &m_font;
  std::vector<Button*> m_buttons;
  sf::Vector2f m_size;
  std::vector<sf::Vector2f> m_sizes;
  sf::Color m_bg_color;

  virtual void updateSprite() override;
  void rebuildMenu(); // Refaire les données graphiques, suite à l'ajout d'un bouton
public:
  Menu(const sf::Font &font, sf::Color bg_color);
  ~Menu();
  void addButton (const std::string &text, sf::Color color, sf::Vector2f margin={},
      sf::Vector2f padding={});
  void setPosition(float x, float y);

  virtual void draw(sf::RenderTexture &) const override;
  std::string wasIClicked(const sf::Event &event) const;
};

#endif /* !F_MENU_H */
