#ifndef F_MAIN_H
#define F_MAIN_H

#include <cmath>
#include <SFML/Graphics.hpp>

// Rend invisible les pixels à l'extérieur d'un disque aussi gros
// que possible dans l'image rectangulaire
inline void makeADisk ( sf::Image &img )
{
  int r = img.getSize().x /2; // Rayon
  for ( int i=0; i < 2*r; i++ )
    for ( int j = 0; j < 2*r; j++ )
      if ( ( i-r ) * ( i-r ) + ( j-r ) * ( j-r ) > r*r )
        img.setPixel ( i,j,sf::Color ( 0,0,0,0 ) );
}

inline sf::Vector2f operator- (const sf::Vector2f &l, const sf::Vector2f &r)
{
  sf::Vector2f t;
  t.x = l.x - r.x;
  t.y = l.y - r.y;
  return t;
}

inline sf::Vector2f operator+ (const sf::Vector2f &l, const sf::Vector2f &r)
{
  sf::Vector2f t;
  t.x = l.x + r.x;
  t.y = l.y + r.y;
  return t;
}

inline sf::Vector2f operator/ (const sf::Vector2f &l, float r)
{
  sf::Vector2f t;
  t.x = l.x/r;
  t.y = l.y/r;
  return t;
}

inline sf::Vector2f operator* (float r, const sf::Vector2f &l)
{
  sf::Vector2f t;
  t.x = l.x*r;
  t.y = l.y*r;
  return t;
}

// Renvoie la norme2 au carré de v
inline float abs2 (const sf::Vector2f &v)
{
  return v.x*v.x + v.y*v.y;
}

// Renvoie la norme2 de v
inline float norm2 (const sf::Vector2f &v)
{
  return std::sqrt(v.x*v.x + v.y*v.y);
}

inline float pow2 (float a)
{
  return a*a;
}

inline float dotProduct (const sf::Vector2f &l, const sf::Vector2f &r)
{
  return l.x*r.x + l.y*r.y;
}

// Calcule le symétrique de v par rapport à axis
inline sf::Vector2f symetric (const sf::Vector2f &v, const sf::Vector2f &axis)
{
  sf::Vector2f norm; // Vecteur normal à axis
  norm.x = axis.y;
  norm.y = -axis.x;
  return v - 2*dotProduct(v,norm)/abs2(norm)*norm;
}

/* Taille du plateau en px */
constexpr int WIDTH = 800;
constexpr int HEIGHT = 500;

/* Taille d'un slime en px */
constexpr int SLIME_WIDTH = WIDTH/8;
constexpr int SLIME_HEIGHT = SLIME_WIDTH/2;
constexpr int PUPIL_RADIUS = 5;
// Maximise la taille de l'œil avec les coordonnées choisies
constexpr int EYE_RADIUS_SQUARED = (int)(SLIME_WIDTH*SLIME_WIDTH* (3.-2.*sqrt ( 2. ) ) /8);
constexpr int EYE_RADIUS = (int)sqrt(EYE_RADIUS_SQUARED);

/* Taille du filet */
constexpr int NET_WIDTH = WIDTH/100;
constexpr int NET_HEIGHT = SLIME_HEIGHT*2;

/* Taille de la balle */
constexpr int BALL_RADIUS = 10;

/* Paramètres physiques */
constexpr float JUMP_DURATION = 1; // Durée du saut en s
constexpr int JUMP_MAX_HEIGHT = HEIGHT/8;
constexpr int SLIME_HORIZONTAL_SPEED = 240;
constexpr float BALL_ELASTICITY = .9f; // Elasticité de la balle, entre 0 et 1
// Calculés automatiquement
constexpr int GRAVITY = 8*JUMP_MAX_HEIGHT/JUMP_DURATION/JUMP_DURATION;
constexpr int SLIME_JUMP_SPEED = 4*JUMP_MAX_HEIGHT/JUMP_DURATION;

#endif
