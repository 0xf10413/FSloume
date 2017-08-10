#ifndef F_GAME_H
#define F_GAME_H

#include <SFML/Graphics.hpp>

#include "slime.h"
#include "ball.h"
#include "net.h"
#include "input.h"

class FGame : public sf::RenderWindow
{
private:
  sf::Event m_event;
  sf::Clock m_clock;
  sf::Font m_font;
  Input m_input;
  bool m_reinit; // Faut-il tout replacer ?

  Slime m_bSlime, m_rSlime;
  Ball m_ball;
  Net m_net;

  void collide (float); // Calcul des collisions et des vitesses
  //void moveAndUpdate ();  // Déplacements effectifs
public:
  FGame ();

  int mainLoop ();
};

#endif // defined F_GAME_H
