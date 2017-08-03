#include "game.h"

/*** BUG ***/
/*
 * Collision par dessous : un slime entre en collision
 * de façon circulaire avec la balle par en-dessous.
 * Balle fantôme : la balle peut passer à travers le filet
 * si elle va assez vite. Solutions : examiner x+vx*dt, majorer vx.
 * */
/*** TODO ***/
/*
 * Une IA
 * Un menu d'accueil
 * Une gestion des scores
 * Des ralentisseurs/objets ?
 */


int main ()
{
  FGame game;
  return game.mainLoop();
}
