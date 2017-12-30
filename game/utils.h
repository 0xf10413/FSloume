#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cmath>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Image.hpp>

// Rend invisible les pixels à l'extérieur d'un disque aussi gros
// que possible dans l'image rectangulaire, et colorie éventuellement
// une bordure supplémentaire
inline void makeADisk (sf::Image &img, int outline = 0,
    sf::Color outline_color = sf::Color(0,0,0,127))
{
  int R = img.getSize().x /2; // Rayon grand disque
  int r = R - outline; // Rayon interne de la couronne
  for (int i=0; i < 2*R; i++)
    for (int j = 0; j < 2*R; j++)
    {
      int ii = i - R;
      int jj = j - R;
      if (ii*ii + jj*jj > R*R)
        img.setPixel (i, j, sf::Color (0, 0, 0, 0));
      else if (ii*ii + jj*jj > r*r)
        img.setPixel(i, j, outline_color);
    }
}

/* Fonctions de collisions */

float collideTwoCircles (sf::Vector2f M, float R, sf::Vector2f V,
    sf::Vector2f Mp, float Rp, sf::Vector2f Vp, float dt);

bool collideCircleWithPoint(sf::Vector2f M, float R, sf::Vector2f V,
    sf::Vector2f Mp, float dt);

sf::Vector2f collideWithFixRectangle (sf::FloatRect rect,
   sf::Vector2f Mp, float R, sf::Vector2f Vp, float dt);

bool collideTwoSegments(sf::Vector2f A, sf::Vector2f B,
    sf::Vector2f C, sf::Vector2f D);

sf::Vector2f uncrushCircleFromRecAndCircle(sf::Vector2f M, float R, sf::Vector2f Vp,
    sf::Vector2f Mp, float Rp);

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

/* Utilitaires sur les std::string */

// a est-elle un préfixe de b ?
// Source : https://stackoverflow.com/questions/7913835/check-if-one-string-is-a-prefix-of-another
inline bool isPrefix (const std::string &a, const std::string &b)
{
  auto res = std::mismatch(a.begin(), a.end(), b.begin());
  return res.first == a.end();
}


#endif /* !UTILS_H */
