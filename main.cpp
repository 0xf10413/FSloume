#include "game/game2.h"
#include "game/game.h"
#include "game/config.h"

int main ()
{
  Config();
  FGame game;
  //Game2 game;
  return game.mainLoop();
}
