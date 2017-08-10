#ifndef F_INPUT_H
#define F_INPUT_H

#include <SFML/Graphics.hpp>
#include <map>

class Input {
private:
  std::map<int, bool> keyDowns;
public:
  Input();
  void process (const sf::Event &e);
  bool isKeyDown (int i) const;
};


#endif // defined F_INPUT_H
