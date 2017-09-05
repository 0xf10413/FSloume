#include "game/game.h"

/*** BUG ***/
/*
 * Collision par dessous : un slime entre en collision
 * de façon circulaire avec la balle par en-dessous.
 * Balle gluante : la balle peut se retrouver coincée sur un sloume
 * */
/*** TODO ***/
/*
 * Une IA
 * Utiliser une macro dans le resource manager
 * Des ralentisseurs/objets ?
 */


int main ()
{
  FGame game;
  return game.mainLoop();
}
