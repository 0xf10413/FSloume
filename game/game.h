#ifndef F_GAME_H
#define F_GAME_H

#include <SFML/Graphics.hpp>

#include "slime.h"
#include "ball.h"
#include "net.h"
#include "input.h"
#include "menu.h"
#include "counter.h"

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

  Menu *m_menu;

  enum GameMode {TITLE, TWO_PLAYERS, ONE_PLAYER};
  GameMode m_game_mode;

  enum BranchMode {PLAYING, BLUE_LOST, RED_LOST};
  BranchMode m_branch_mode;

  sf::Time m_game_over_countdown;
  Counter m_lScore, m_rScore;

  sf::Text m_gameOverText;

  void collide (float); // Calcul des collisions et des vitesses
  //void moveAndUpdate ();  // Déplacements effectifs
  void rebuildGame();
public:
  FGame ();
  FGame& operator=(const FGame &) = delete;
  FGame(const FGame &) = delete;
  virtual ~FGame() = default;

  int mainLoop ();
};

#endif // defined F_GAME_H
