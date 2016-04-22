#ifndef F_BALL_H
#define F_BALL_H

#include <SFML/Graphics.hpp>
#include "main.h"

class Ball {
private:
    sf::Texture texture;
	sf::Image image;
	sf::Sprite sprite;
public:
	float x, y;
	float vx, vy;
	Ball ();
	void setX (float nx) { sprite.setPosition(x = nx, sprite.getPosition().y); }
	void setY (float ny) { sprite.setPosition (sprite.getPosition().x, y = ny);}
	const sf::Sprite &getSprite () {return sprite;}
	sf::Vector2f getCenter ();
};

#endif
