#include "game/game.h"

/*** BUG ***/
/*
 * Collision par dessous : un slime entre en collision
 * de façon circulaire avec la balle par en-dessous.
 * Balle gluante : la balle peut se retrouver coincée sur un sloume
 * Config générale : passer par un générateur de cpp au lieu de variables
 * globales.
 * Config générale : problème de l'initialisation
 * Config générale : résolution sur PC trop grande
 * */
/*** TODO ***/
/*
 * Une IA
 * Utiliser une macro dans le resource manager
 * Des ralentisseurs/objets ?
 */


int main ()
{
  Config();
  FGame game;
  return game.mainLoop();
}
