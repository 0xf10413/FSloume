#ifndef F_GAME_H
#define F_GAME_H

#include <array>
#include <SFML/Graphics.hpp>

#include "entities/slime.h"
#include "entities/ball.h"
#include "entities/net.h"
#include "tools/input.h"
#include "ui/menu.h"
#include "ui/counter.h"
#include "rc_manager.h"
#include "ui/target.h"
#include "entities/dangerpt.h"
#include "ui/background.h"
#include "entities/shockwave.h"

class FGame : public sf::RenderWindow
{
private:
  sf::Event m_event;
  sf::Clock m_clock;
  std::shared_ptr<sf::Font> m_font;
  Input m_input;
  bool m_reinit, m_full_reinit; // Faut-il tout replacer ?
  bool m_paused, m_defer_unlock_pause;


  Background m_background;
  Slime m_bSlime, m_rSlime;
  Ball m_ball;
  Net m_net;

  Menu *m_main_menu, *m_pause_menu;

  enum GameMode {TITLE, TWO_PLAYERS, ONE_PLAYER, TEST};
  GameMode m_game_mode;

  enum BranchMode {PLAYING, BLUE_LOST, RED_LOST};
  BranchMode m_branch_mode;

  sf::Time m_game_over_countdown;
  Counter m_lScore, m_rScore;

  sf::Text m_gameOverText;
  std::array<Target,10> m_targets;

  DangerPoint m_dangerpt;
  ShockWave m_shockwaves[2];
  ParticleGenerator m_pgenerator;

  void collide (float dt, bool fake=false); // Calcul des collisions et des vitesses
  //void moveAndUpdate ();  // Déplacements effectifs
  void rebuildGame();
public:
  FGame ();
  FGame& operator=(const FGame &) = delete;
  FGame(const FGame &) = delete;
  virtual ~FGame();

  int mainLoop ();
};

#endif // defined F_GAME_H
