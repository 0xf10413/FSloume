#include "counter.h"


Counter::Counter(size_t init, bool alignLeft, const sf::Font &font) :
  m_count(init),
  m_alignLeft(alignLeft),
  m_font(font),
  m_text()
{
  m_text.setFont(m_font);
  if (m_alignLeft)
    m_text.setFillColor(sf::Color::Cyan);
  else
    m_text.setFillColor(sf::Color::Red);
}

Counter &Counter::operator++()
{
  m_count++;
  updateSprite();
  return *this;
}

Counter &Counter::operator--()
{
  m_count--;
  updateSprite();
  return *this;
}

void Counter::setX(float x)
{
  m_x = x;
  updateSprite();
}

void Counter::setY(float y)
{
  m_y = y;
  updateSprite();
}

void Counter::setValue(size_t value)
{
  m_count = value;
  updateSprite();
}

void Counter::setPosition(float x, float y)
{
  m_x = x;
  m_y = y;
  updateSprite();
}


void Counter::updateSprite()
{
  m_text.setString(std::to_string(m_count));
  auto shape = m_text.getLocalBounds();
  if (m_alignLeft) // Position = coin supérieur gauche
  {
    m_text.setPosition(
        m_x,
        m_y
        );
  }
  else // Position = coin supérieur gauche
  {
    m_text.setPosition(
        m_x - shape.left - shape.width,
        m_y
        );
  }
}

void Counter::draw (sf::RenderWindow &w) const
{
  w.draw(m_text);
}
