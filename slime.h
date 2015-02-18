#ifndef F_SLIME_H
#define F_SLIME_H

#include <SFML/Graphics.hpp>
#include "main.h"

class Slime {
private:
	sf::Image image;
	sf::Sprite sprite;
	sf::Image eyeImage;
	sf::Sprite eyeSprite;
	bool left;
public:
	float x, y;
	float vx, vy;
	bool onGround;
	Slime (bool isLeft);
	void setX (float nx) { sprite.SetX(x = nx); eyeSprite.SetX ( sprite.GetPosition().x + 3*slimeWidth/4 - pupilRadius );}
	void setY (float ny) { sprite.SetY (y = ny);eyeSprite.SetY ( sprite.GetPosition().y+slimeHeight/2-pupilRadius );}
	const sf::Sprite &getSprite () {return sprite;}
	const sf::Sprite &getEye () { return eyeSprite;}
	sf::Vector2f getCenter ();
	void updateEye (const sf::Vector2f &); // Reçoit le centre de la balle
};

#endif