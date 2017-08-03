#ifndef F_MAIN_H
#define F_MAIN_H

#include <cmath>
#include <SFML/Graphics.hpp>

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

const int width = 800;
const int height = 500;
const int slimeWidth = width/8;
const int slimeHeight = slimeWidth/2;
const int netWidth = width/100;
const int netHeight = slimeHeight*2;
const int ballRadius = 10;
/* Maximise la taille de l'œil avec les coordonnées choisies */
const int eyeRadiusSquared = static_cast<int>(slimeWidth*slimeWidth* ( 3.-2.*sqrt ( 2. ) ) /8);
const int pupilRadius = 5;
const int gravity = 1; // Intensité de pesanteur
const int slimeHorizontalSpeed = 140;

#endif
