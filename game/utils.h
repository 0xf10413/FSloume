#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Image.hpp>

// Rend invisible les pixels à l'extérieur d'un disque aussi gros
// que possible dans l'image rectangulaire
inline void makeADisk ( sf::Image &img )
{
  int r = img.getSize().x /2; // Rayon
  for (int i=0; i < 2*r; i++)
    for (int j = 0; j < 2*r; j++)
      if ((i - r) * (i - r) + (j - r) * (j - r) > r*r)
        img.setPixel (i, j, sf::Color (0, 0, 0, 0));
}

/* Fonctions de collisions */

float collideTwoCircles (sf::Vector2f M, float R, sf::Vector2f V,
    sf::Vector2f Mp, float Rp, sf::Vector2f Vp, float dt);

sf::Vector2f collideWithFixRectangle (sf::FloatRect rect,
   sf::Vector2f Mp, float R, sf::Vector2f Vp, float dt);

bool collideTwoSegments(sf::Vector2f A, sf::Vector2f B,
    sf::Vector2f C, sf::Vector2f D);

bool isTriangleDirect (sf::Vector2f A, sf::Vector2f B, sf::Vector2f C);


/* Utilitaires sur les sf::Vector2f */
std::ostream &operator<< (std::ostream &out, const sf::Vector2f& v);

bool operator <= (sf::Vector2f A, sf::Vector2f B);


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

inline float det(sf::Vector2f l, sf::Vector2f r)
{
  return l.x*r.y - l.y*r.x;
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


#endif /* !UTILS_H */
