#include "input.h"

Input::Input() : keyDowns(), touchDown(false), touchPlace()
{
  for (int i = 0; i < sf::Keyboard::KeyCount; i++) keyDowns[i] = false;
}

void Input::process (const sf::Event &e)
{
  if (e.type == sf::Event::KeyPressed)
    keyDowns[e.key.code] = true;
  else if (e.type == sf::Event::KeyReleased)
    keyDowns[e.key.code] = false;
  else if (e.type == sf::Event::TouchBegan)
  {
    touchPlace.x = (float)e.touch.x;
    touchPlace.y = (float)e.touch.y;
    touchDown = true;
  }
  else if (e.type == sf::Event::TouchEnded)
  {
    touchDown = false;
  }
  else if (e.type == sf::Event::TouchMoved)
  {
    touchPlace.x = (float)e.touch.x;
    touchPlace.y = (float)e.touch.y;
  }
}

bool Input::isKeyDown (int i) const
{
  return keyDowns.at(i);
}

bool Input::isTouchDown () const
{
  return touchDown;
}

sf::Vector2f Input::whereIsTouch() const
{
  return touchPlace;
}

