#ifndef F_GAME_H
#define F_GAME_H

#include <SFML/Graphics.hpp>

#include "slime.h"
//#include "ball.h"
//#include "net.h"
#include "input.h"

class FGame : public sf::RenderWindow
{
private:
  sf::Event event;
  sf::Clock clock;
  sf::Font font;
  Input input;
  bool reinit; // Faut-il tout replacer ?

  Slime bSlime;//, rSlime;
  //Net net;
  //Ball ball;

  void collide (float); // Calcul des collisions et des vitesses
  void moveAndUpdate ();  // Déplacements effectifs
public:
  FGame ();

  int mainLoop ();
};

#endif // defined F_GAME_H
