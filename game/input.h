#ifndef F_INPUT_H
#define F_INPUT_H

#include <SFML/Graphics.hpp>
#include <map>

class Input {
private:
  std::map<int, bool> keyDowns;
  static const size_t maxFingers = 5;
  bool touchDowns[maxFingers];
  sf::Vector2f touchPlaces[maxFingers];
public:
  Input();
  void process (const sf::Event &e);
  bool isKeyDown (int i) const;
  bool isTouchDown () const;
  sf::Vector2f whereIsTouch() const;
};


#endif // defined F_INPUT_H
