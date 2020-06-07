#include "button.h"
#include "config.h"
#include <cassert>


Button::Button(const sf::Font &font, const std::string &text, sf::Color color,
    sf::Vector2f margin, sf::Vector2f pad) :
  m_font(font),
  m_text(text, m_font),
  m_color(color),
  m_textSize(),
  m_margin(margin),
  m_padding(pad)
{
  m_text.setCharacterSize(CG::FONT_BASE_SIZE_PX/2);
  m_text.setFillColor(CG::MENU_TEXT_COLOR);
  sf::FloatRect bounds = m_text.getGlobalBounds();
  m_textSize.x = bounds.left + bounds.width + bounds.left;
  m_textSize.y = bounds.top + bounds.height + bounds.top;

  m_image.create(
      (unsigned int)(m_textSize.x + m_padding.x),
      (unsigned int)(m_textSize.y + m_padding.y),
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
      m_x - m_textSize.x/2,
      m_y - m_textSize.y/2
      );
}

void Button::setPosition(float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}

void Button::draw(sf::RenderTexture &w) const
{
  w.draw(m_sprite);
  w.draw(m_text);
}

bool Button::wasIClicked(const sf::Event &event) const
{
  if (event.type == sf::Event::MouseButtonPressed)
    return m_sprite.getGlobalBounds().contains(
        (float)event.mouseButton.x,
        (float)event.mouseButton.y
        );
  else if (event.type == sf::Event::TouchEnded)
    return m_sprite.getGlobalBounds().contains(
        (float)event.touch.x,
        (float)event.touch.y
        );
  throw std::runtime_error("Called with wrong event type");
}

sf::Vector2f Button::getSize() const
{
  return m_textSize;
}

std::string Button::getTextString() const
{
  return m_text.getString();
}


sf::Vector2f Button::getTotalSize() const
{
  return m_textSize + m_margin + m_padding;
}
