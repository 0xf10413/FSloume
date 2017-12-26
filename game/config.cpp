#include "config.h"
#include <SFML/Window/VideoMode.hpp>


Config::Config()
{
  /* Taille du plateau en px */
  WIDTH = 900;
  HEIGHT = 480;

#ifdef F_CONFIG_ANDROID
  WIDTH = (signed)sf::VideoMode::getDesktopMode().width;
  HEIGHT = (signed)sf::VideoMode::getDesktopMode().height;
#endif

  updateParams();
}

void Config::updateParams()
{
  /* Taille d'un slime en px */
  SLIME_WIDTH = WIDTH/8;
  SLIME_HEIGHT = SLIME_WIDTH/2;

  /* Taille de base de la police */
  FONT_BASE_SIZE_PX = (unsigned int)HEIGHT/10;

  // Maximise la taille de l'œil avec les coordonnées choisies
  EYE_RADIUS_SQUARED = (int)(SLIME_WIDTH*SLIME_WIDTH* (3.-2.*sqrt ( 2. ) ) /8);
  EYE_RADIUS = (int)sqrt(EYE_RADIUS_SQUARED);
  PUPIL_RADIUS = EYE_RADIUS/3;

  /* Taille du filet */
  NET_WIDTH = WIDTH/15;
  NET_HEIGHT = SLIME_HEIGHT*2;

  /* Taille de la balle */
  BALL_RADIUS = SLIME_HEIGHT/6;

  /* Taille du panneau de danger */
  DANGERPT_WIDTH = 2*BALL_RADIUS;

  CLOUD_WIDTH = WIDTH/10;
  CLOUD_HEIGHT = HEIGHT/10;

  /* Taille des mires */
  TARGET_WIDTH = WIDTH/8;
  TARGET_HEIGHT = HEIGHT/8;

  /* Pas d'anticipation sur le mouvement de la balle */
  BALL_ANTICIPATION = 100;

  /* Paramètres physiques */
  JUMP_DURATION = 1; // Durée du saut en s
  JUMP_MAX_HEIGHT = HEIGHT/8;
  SLIME_HORIZONTAL_SPEED = WIDTH/5;
  SLIME_TIME_BOOST = .2f; // Temps en s où on peut booster
  SLIME_DOUBLE_JUMP_TIME = .3f; // Temps en s avant de pouvoir double-jump
  SLIME_HEIGHT_GROUND_POUND = HEIGHT - NET_HEIGHT; // Hauteur min pour une charge au sol
  BALL_ELASTICITY = .9f; // Elasticité de la balle, entre 0 et 1
  SLIME_ELASTICITY = .9f; // Elasticité du sloume, entre 0 et 1
  // Calculés automatiquement
  GRAVITY = (int)(8*JUMP_MAX_HEIGHT/JUMP_DURATION/JUMP_DURATION);
  SLIME_JUMP_SPEED = (int)(4*JUMP_MAX_HEIGHT/JUMP_DURATION);
}

int Config::WIDTH;
int Config::HEIGHT;

const size_t Config::NB_MAX_FINGERS;



unsigned int Config::FONT_BASE_SIZE_PX;

int Config::SLIME_WIDTH;
int Config::SLIME_HEIGHT;
int Config::PUPIL_RADIUS;
int Config::EYE_RADIUS_SQUARED;
int Config::EYE_RADIUS;

int Config::NET_WIDTH;
int Config::NET_HEIGHT;

int Config::BALL_RADIUS;

int Config::DANGERPT_WIDTH;

int Config::CLOUD_WIDTH;
int Config::CLOUD_HEIGHT;

unsigned int Config::TARGET_WIDTH;
unsigned int Config::TARGET_HEIGHT;

int Config::BALL_ANTICIPATION;

float Config::JUMP_DURATION; // Durée du saut en s
int Config::JUMP_MAX_HEIGHT;
int Config::SLIME_HORIZONTAL_SPEED;
float Config::SLIME_TIME_BOOST; // Temps en s où on peut booster
float Config::SLIME_DOUBLE_JUMP_TIME; // Temps en s avant de pouvoir double-jump
float Config::BALL_ELASTICITY; // Elasticité de la balle, entre 0 et 1
float Config::SLIME_ELASTICITY; // Elasticité du sloume, entre 0 et 1

float Config::GRAVITY;
int Config::SLIME_JUMP_SPEED;

float Config::SLIME_HEIGHT_GROUND_POUND; // Hauteur min pour une charge au sol
