#ifndef F_BALL_H
#define F_BALL_H

#include <SFML/Graphics.hpp>
#include "main.h"

class Ball {
private:
	sf::Image image;
	sf::Sprite sprite;
public:
	float x, y;
	float vx, vy;
	Ball ();
	void setX (float nx) { sprite.SetX(x = nx); }
	void setY (float ny) { sprite.SetY (y = ny);}
	const sf::Sprite &getSprite () {return sprite;}
	sf::Vector2f getCenter ();
};

#endif