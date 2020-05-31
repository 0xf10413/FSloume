#include "utils.h"
#include "config.h"
#include <cmath>
#include <iostream>

/* Intersection entre deux cercles mouvants. Nomenclature :
 * Premier cercle : centre M, rayon R, vitesse V
 * Second cercle : centre M' (noté Mp), rayon R' (noté Rp), vitesse V' (Vp)
 * pour un temps allant de 0 à dt
 * Renvoie le temps de collision s'il existe, -1 sinon
 **/
float collideTwoCircles (sf::Vector2f M, float R, sf::Vector2f V,
    sf::Vector2f Mp, float Rp, sf::Vector2f Vp, float dt)
{
  /* On montre que l'existence d'un temps d'intersection équivaut à
   * trouver la plus petite racine d'un trinôme */
  sf::Vector2f delta_v = Vp - V;
  float RppR2 = pow2(R+Rp);
  sf::Vector2f MpM = Mp - M;
  float a = abs2(delta_v);
  float b = 2*dotProduct(MpM, delta_v);
  float c = abs2(MpM) - RppR2;

  if (a == 0.) // Pas de trinôme
  {
    if (b == 0.) // Pas de solution
      return -1;
    float t = -c/b;
    if (t < 0. || t > dt) // Hors de l'intervalle considéré
      return -1;
    return -c/b;
  }

  float delta = b*b - 4*a*c;
  if (delta < 0) // Pas de solution
    return -1.;

  float t_min = (-b - std::sqrt(delta)) / (2*a);
  float t_max = (-b + std::sqrt(delta)) / (2*a);
  if (t_min > dt || t_max < 0.) // Hors de l'intervalle considéré
    return -1;
  return std::max(0.f, t_min);
}

bool operator <= (sf::Vector2f A, sf::Vector2f B)
{
  return (A.x < B.x) || (A.x == B.x && A.y < B.y);
}

bool isinf (sf::Vector2f A)
{
  return std::isinf(A.x) || std::isinf(A.y);
}

bool isNaN (sf::Vector2f A)
{
  return std::isnan(A.x) || std::isnan(A.y);
}

/* Renvoie true ssi ABC est un triangle direct */
bool isTriangleDirect (sf::Vector2f A, sf::Vector2f B, sf::Vector2f C)
{
  /* Si un des points est indéfini, le triangle l'est aussi
   * Le triangle n'est donc pas direct (mais pas indirect non plus)
   */
  if (isNaN(A) || isNaN(B) || isNaN(C))
    return false;

  /* Pour les triangles infinis, c'est délicat
   * Pour l'instant, on fait pareil qu'avec les triangles indéfinis */
  if (isinf(A) || isinf(B) || isinf(C))
    return false;

  /* ABC non plat est direct ssi l'angle orienté AB,AC est positif */
  /* s'il est plat, il est direct ssi les points sont rangés
   * dans le même ordre */
  float AB_AC = det(B-A, C-A);

  if (AB_AC > 0)
    return true;
  if (AB_AC < 0)
    return false;
  if (AB_AC == 0)
    return (A <= B && B <= C) || (B <= C && C <= A) || (C <= A && A <= B);
  return false;
}

/* Renvoie t tel que A + tAB soit le pt d'intersection des deux
 * segments [AB] et [CD] s'il existe et est unique, -1 sinon
 * … dans une version future. Pour l'instant, check juste l'intersection
 */
bool collideTwoSegments(sf::Vector2f A, sf::Vector2f B,
    sf::Vector2f C, sf::Vector2f D)
{
  return (isTriangleDirect(A, B, C) == isTriangleDirect(A, D, B))
    && (isTriangleDirect(D, B, C) == isTriangleDirect(A, D, C));
}

/* Check si un cercle mouvant passe par un point.
 * Nomenclature :
 * * Cercle mouvant de centre M, rayon R, vitesse V
 * * Point fixe de coordonées M' (noté Mp)
 */
bool collideCircleWithPoint(sf::Vector2f M, float R, sf::Vector2f V,
    sf::Vector2f Mp, float dt)
{
  sf::Vector2f MMp = Mp-M;
  float a = abs2(V);
  float b = -2*dotProduct(MMp, V);
  float c = abs2(MMp) - R*R;

  if (a == 0)
  {
    if (b == 0)
      return false;
    return true; // -c/b
  }

  float delta = b*b - 4*a*c;
  if (delta < 0)
    return false;
  float t_min = (-b - std::sqrt(delta))/(2*a);
  float t_max = (-b + std::sqrt(delta))/(2*a);

  return t_max >= 0 && t_min <= dt;
}

/* Intersection entre un rectangle fixe et un cercle mouvant.
 * Nomenclature :
 *  Rectangle : décrit par un sf::FloatRect
 *  Cercle : centre M' (noté Mp), rayon R' (noté Rp), vitesse V (Vp)
 * pour un temps allant de 0 à dt
 * Renvoie le temps de collision s'il existe, -1 sinon
 **/
sf::Vector2f collideWithFixRectangle (sf::FloatRect rect,
   sf::Vector2f Mp, float R, sf::Vector2f Vp, float dt)
{
  /* Points remarquables du disque */
  /*   B
   *  / \
   * A   C
   *  \ /
   *   D
   */
  sf::Vector2f Ap = Mp; Ap.x -= R; // gauche
  sf::Vector2f Bp = Mp; Bp.y -= R; // haut
  sf::Vector2f Cp = Mp; Cp.x += R; // droite
  sf::Vector2f Dp = Mp; Dp.y += R; // bas

  sf::Vector2f Mp_tf = Mp + dt*Vp; // Mp final
  sf::Vector2f Ap_tf = Mp_tf; Ap_tf.x -= R;
  sf::Vector2f Bp_tf = Mp_tf; Bp_tf.y -= R;
  sf::Vector2f Cp_tf = Mp_tf; Cp_tf.x += R;
  sf::Vector2f Dp_tf = Mp_tf; Dp_tf.y += R;

  /* Bords de la boîte */
  /* A--B
   * |  |
   * D--C
   */
  sf::Vector2f A (rect.left, rect.top);
  sf::Vector2f B (rect.left + rect.width, rect.top);
  sf::Vector2f C (rect.left + rect.width, rect.top + rect.height);
  sf::Vector2f D (rect.left, rect.top + rect.height);

  /* Deux parties : collision avec un sommet ou une arête */
  /* Quatres sous-parties à chaque fois */

  /* Collision arête haut */
  /* test sur le point bas du disque */
  if (collideTwoSegments(A, B, Dp, Dp_tf))
    return {0, -1};

  /* Collision arête droite */
  /* test sur le point gauche du disque */
  if (collideTwoSegments(B, C, Ap, Ap_tf))
    return {1, 0};

  /* Collision arête bas */
  /* test sur le point haut du disque */
  if (collideTwoSegments(C, D, Bp, Bp_tf))
    return {0, 1};

  /* Collision arête gauche */
  /* test sur le point droit du disque */
  if (collideTwoSegments(D, A, Cp, Cp_tf))
    return {-1, 0};

  /* Collision sommet haut gauche */
  if (collideCircleWithPoint(Mp, R, Vp, A, dt))
    return {-1, -1};

  /* Collision sommet haut droite */
  if (collideCircleWithPoint(Mp, R, Vp, B, dt))
    return {1, -1};

  /* Collision sommet bas droite */
  if (collideCircleWithPoint(Mp, R, Vp, C, dt))
    return {1, 1};

  /* Collision sommet bas gauche */
  if (collideCircleWithPoint(Mp, R, Vp, D, dt))
    return {-1, 1};

  return {0,0};
}

float clamp(float value, float min, float max)
{
  if (value <= min)
    return min;
  if (value >= max)
    return max;
  return value;
}

// TODO: référencer, unit-tester
// Source : https://stackoverflow.com/questions/401847/circle-rectangle-collision-detection-intersection
bool isInsideRectangle(sf::Vector2f M, float R, sf::FloatRect rect)
{
  // Find the closest point to the circle within the rectangle
  float closestX = clamp(M.x, rect.left, rect.left + rect.width);
  float closestY = clamp(M.x, rect.top, rect.top + rect.height);

  // Calculate the distance between the circle's center and this closest point
  float distanceX = M.x - closestX;
  float distanceY = M.y - closestY;

  // If the distance is less than the circle's radius, an intersection occurs
  float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
  return distanceSquared < (R * R);
}

/* Débloque un cercle s'il se retrouve dans un cercle et / ou un rectangle */
// TODO: unit tester
sf::Vector2f uncrushCircleFromRecAndCircle(sf::Vector2f M, float R, sf::Vector2f Vp,
    sf::Vector2f Mp, float Rp)
{
  /* Première étape : sortir le cercle de l'autre */
  sf::Vector2f finalDir = Vp;

  if (abs2(M-Mp) < pow2(R+Rp))
  {
    sf::Vector2f dir = M - Mp;
    dir /= norm2(dir);
    dir *= norm2(Vp);
    finalDir = dir;
  }

  return finalDir;
}

std::ostream &operator<< (std::ostream &out, const sf::Vector2f &v)
{
  return out << "(" << v.x << ", " << v.y << ")";
}
