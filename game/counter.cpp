#include "counter.h"
#include "config.h"
#include <sstream>

Counter::Counter(size_t init, bool alignLeft, const sf::Font &font) :
  m_count(init),
  m_alignLeft(alignLeft),
  m_font(font),
  m_text()
{
  m_text.setFont(m_font);
  m_text.setCharacterSize(CG::FONT_BASE_SIZE_PX);
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

void Counter::reinit()
{
  m_count = 0;
  updateSprite();
}


void Counter::updateSprite()
{
  std::ostringstream oss;
  oss << m_count;
  m_text.setString(oss.str());
  auto shape = m_text.getLocalBounds();
  if (m_alignLeft) // Position = coin supérieur gauche
  {
    m_text.setPosition(
        m_x,
        m_y - shape.top
        );
  }
  else // Position = coin supérieur gauche
  {
    m_text.setPosition(
        m_x - shape.left - shape.width,
        m_y - shape.top
        );
  }
}

void Counter::draw (sf::RenderWindow &w) const
{
  w.draw(m_text);
}
