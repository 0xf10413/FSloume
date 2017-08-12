#include "button.h"
#include <cassert>
#include <iostream>


Button::Button(const sf::Font &font, const std::string &text, sf::Color color) :
  m_font(font),
  m_text(text, m_font),
  m_color(color),
  m_size()
{
  sf::FloatRect bounds = m_text.getGlobalBounds();
  m_size.x = bounds.left + bounds.width + bounds.left;
  m_size.y = bounds.top + bounds.height + bounds.top;

  m_image.create(
      (unsigned int)(m_size.x),
      (unsigned int)(m_size.y),
      m_color
      );
  m_texture.loadFromImage(m_image);
  m_sprite.setTexture(m_texture);
}

void Button::updateSprite()
{
  m_sprite.setPosition(
      m_x - m_sprite.getGlobalBounds().width/2,
      m_y - m_sprite.getGlobalBounds().height/2
      );
  m_text.setPosition(
      m_x - m_sprite.getGlobalBounds().width/2,
      m_y - m_sprite.getGlobalBounds().height/2
      );
}

void Button::setPosition(float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

void Button::draw(sf::RenderWindow &w) const
{
  w.draw(m_sprite);
  w.draw(m_text);
}

bool Button::wasIClicked(const sf::Event &event) const
{
  assert (event.type == sf::Event::MouseButtonPressed);
  return m_sprite.getGlobalBounds().contains(
        (float)event.mouseButton.x,
        (float)event.mouseButton.y
        );
}

sf::Vector2f Button::getSize() const
{
  return m_size;
}

std::string Button::getTextString() const
{
  return m_text.getString();
}
