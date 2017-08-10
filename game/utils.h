#ifndef UTILS_H
#define UTILS_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <iostream>

float collideTwoCircles (sf::Vector2f M, float R, sf::Vector2f V,
    sf::Vector2f Mp, float Rp, sf::Vector2f Vp, float dt);

sf::Vector2f collideWithFixRectangle (sf::FloatRect rect,
   sf::Vector2f Mp, float R, sf::Vector2f Vp, float dt);

bool collideTwoSegments(sf::Vector2f A, sf::Vector2f B,
    sf::Vector2f C, sf::Vector2f D);

bool isTriangleDirect (sf::Vector2f A, sf::Vector2f B, sf::Vector2f C);

std::ostream &operator<< (std::ostream &out, const sf::Vector2f& v);

#endif /* !UTILS_H */
