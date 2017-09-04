#ifndef F_MAIN_H
#define F_MAIN_H

#include <cmath>
#include <SFML/Graphics.hpp>
#include <iostream>

/* Taille du plateau en px */
constexpr int WIDTH = 900;
constexpr int HEIGHT = 500;

/* Taille d'un slime en px */
constexpr int SLIME_WIDTH = WIDTH/8;
constexpr int SLIME_HEIGHT = SLIME_WIDTH/2;
constexpr int PUPIL_RADIUS = 5;
// Maximise la taille de l'œil avec les coordonnées choisies
constexpr int EYE_RADIUS_SQUARED = (int)(SLIME_WIDTH*SLIME_WIDTH* (3.-2.*sqrt ( 2. ) ) /8);
constexpr int EYE_RADIUS = (int)sqrt(EYE_RADIUS_SQUARED);

/* Taille du filet */
constexpr int NET_WIDTH = WIDTH/6;
constexpr int NET_HEIGHT = SLIME_HEIGHT*2;

/* Taille de la balle */
constexpr int BALL_RADIUS = 20;

/* Pas d'anticipation sur le mouvement de la balle */
constexpr int BALL_ANTICIPATION = 100;

/* Paramètres physiques */
constexpr float JUMP_DURATION = 1; // Durée du saut en s
constexpr int JUMP_MAX_HEIGHT = HEIGHT/8;
constexpr int SLIME_HORIZONTAL_SPEED = 240;
constexpr float BALL_ELASTICITY = .9f; // Elasticité de la balle, entre 0 et 1
constexpr float SLIME_ELASTICITY = .9f; // Elasticité du sloume, entre 0 et 1
// Calculés automatiquement
constexpr int GRAVITY = 8*JUMP_MAX_HEIGHT/JUMP_DURATION/JUMP_DURATION;
constexpr int SLIME_JUMP_SPEED = 4*JUMP_MAX_HEIGHT/JUMP_DURATION;

#endif
