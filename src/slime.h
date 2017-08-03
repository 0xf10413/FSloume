#ifndef F_SLIME_H
#define F_SLIME_H

#include <SFML/Graphics.hpp>
#include "main.h"

class Slime {
private:
  sf::Texture texture;
	sf::Image image;
	sf::Sprite sprite;

  sf::Texture eyeTexture;
	sf::Image eyeImage;
	sf::Sprite eyeSprite;
	bool left;
public:
	float x, y;
	float vx, vy;
	bool onGround, lost;
	int victories;
	Slime (bool isLeft);
	void setX (float nx) { sprite.setPosition (x = nx, sprite.getPosition().y); eyeSprite.setPosition ( sprite.getPosition().x + 3*slimeWidth/4 - pupilRadius, eyeSprite.getPosition().y );}
	void setY (float ny) { sprite.setPosition (sprite.getPosition().x, y = ny);eyeSprite.setPosition ( sprite.getPosition().x, sprite.getPosition().y+slimeHeight/2-pupilRadius );}
	const sf::Sprite &getSprite () {return sprite;}
	const sf::Sprite &getEye () { return eyeSprite;}
	sf::Vector2f getCenter ();
	void updateEye (const sf::Vector2f &); // Reçoit le centre de la balle
};

#endif
