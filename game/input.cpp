#include "input.h"

Input::Input() : keyDowns()
{
  for (int i = 0; i < sf::Keyboard::KeyCount; i++) keyDowns[i] = false;
}

void Input::process (const sf::Event &e)
{
  if (e.type == sf::Event::KeyPressed)
    keyDowns[e.key.code] = true;
  else if (e.type == sf::Event::KeyReleased)
    keyDowns[e.key.code] = false;
}

bool Input::isKeyDown (int i) const
{
  return keyDowns.at(i);
}
