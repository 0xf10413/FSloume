#ifndef F_INPUT_H
#define F_INPUT_H

#include <SFML/Graphics.hpp>
#include <map>
#include "config.h"

class Input {
private:
  std::map<int, bool> keyDowns;
  bool touchDowns[CG::NB_MAX_FINGERS];
  sf::Vector2f touchPlaces[CG::NB_MAX_FINGERS];
public:
  Input();
  void process (const sf::Event &e);
  bool isKeyDown (int i) const;
  bool isTouchDown () const;
  std::vector<sf::Vector2f> whereAreTouch() const;
};


#endif // defined F_INPUT_H
