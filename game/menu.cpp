#include "menu.h"
#include <iostream>
#include <cassert>
#include <numeric>

Menu::Menu(const sf::Font &font, sf::Color bg_color):
  m_font(font),
  m_buttons(),
  m_size(),
  m_sizes(),
  m_bg_color(bg_color)
{
  m_size.x = 50;
  m_size.y = 1;
  m_image.create((unsigned int)m_size.x, (unsigned int)m_size.y, m_bg_color);
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void Menu::addButton (const std::string &text, sf::Color color, sf::Vector2f margin,
    sf::Vector2f padding)
{
  m_buttons.push_back(new Button (m_font, text, color, margin, padding));
  rebuildMenu();
  updateSprite();
}

void Menu::setPosition(float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

template<typename T>
T sum(const std::vector<T> &v, size_t a, size_t b)
{
  if (b <= a)
    return T();
  if (a > v.size())
    return T();
  b = std::min(v.size()-1, b);
  return std::accumulate(v.begin()+a, v.begin()+b, T());
}

void Menu::updateSprite()
{
  m_sprite.setPosition(m_x - m_size.x/2, m_y - m_size.y/2);
  for (size_t i = 0; i < m_buttons.size(); ++i)
    m_buttons[i]->setPosition(
        m_x,
        m_y - m_size.y/2 + m_sizes[i].y/2 + sum(m_sizes, 0, i).y
        );
}

void Menu::rebuildMenu()
{
  m_size = sf::Vector2f(0,0);
  m_sizes.clear();
  for (size_t i = 0; i < m_buttons.size(); ++i)
  {
    m_sizes.emplace_back(m_buttons[i]->getTotalSize());
    m_size.y += m_sizes[i].y;
    m_size.x = std::max(m_size.x, m_sizes[i].x);
  }
  m_image.create(
      (unsigned int)m_size.x,
      (unsigned int)m_size.y,
      m_bg_color
      );

  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture, true);
}

std::string Menu::wasIClicked(const sf::Event &event) const
{
  assert (event.type == sf::Event::MouseButtonPressed);

  if (m_sprite.getGlobalBounds().contains(
        (float)event.mouseButton.x,
        (float)event.mouseButton.y
        ))
    for (const Button *b : m_buttons)
      if (b->wasIClicked(event))
        return b->getTextString();
  return "";
}

Menu::~Menu()
{
  for (Button *b : m_buttons)
    delete b;
}

void Menu::draw(sf::RenderWindow &w) const
{
  w.draw(m_sprite);
  for (const Button *b : m_buttons)
    b->draw(w);
}

