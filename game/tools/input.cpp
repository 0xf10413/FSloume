#include "tools/input.h"
#include <cassert>

Input::Input() : keyDowns(), touchDowns(), touchPlaces()
{
  for (int i = 0; i < sf::Keyboard::KeyCount; i++) keyDowns[i] = false;
  for (size_t i = 0; i < CG::NB_MAX_FINGERS; ++i) touchDowns[i] = false;
}

void Input::process (const sf::Event &e)
{
  // TODO : ignorer les finger > nb_max_finger
  if (e.type == sf::Event::KeyPressed)
    keyDowns[e.key.code] = true;
  else if (e.type == sf::Event::KeyReleased)
    keyDowns[e.key.code] = false;
  else if (e.type == sf::Event::TouchBegan)
  {
    touchPlaces[e.touch.finger].x = (float)e.touch.x;
    touchPlaces[e.touch.finger].y = (float)e.touch.y;
    touchDowns[e.touch.finger] = true;
  }
  else if (e.type == sf::Event::TouchEnded)
  {
    touchDowns[e.touch.finger] = false;
  }
  else if (e.type == sf::Event::TouchMoved)
  {
    touchPlaces[e.touch.finger].x = (float)e.touch.x;
    touchPlaces[e.touch.finger].y = (float)e.touch.y;
  }
}

bool Input::isKeyDown (int i) const
{
  return keyDowns.at(i);
}

bool Input::isTouchDown () const
{
  return std::find(std::begin(touchDowns), std::end(touchDowns), true) != std::end(touchDowns);
}

std::vector<sf::Vector2f> Input::whereAreTouch() const
{
  size_t i;
  std::vector<sf::Vector2f> touches;
  touches.reserve(3);
  for (i = 0; i < CG::NB_MAX_FINGERS; ++i)
    if (touchDowns[i])
      touches.push_back(touchPlaces[i]);
  return touches;
}

