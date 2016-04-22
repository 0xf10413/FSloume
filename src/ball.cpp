#include "ball.h"

Ball::Ball()
{
	x = y = vx = vy = 0;
	image.create(ballRadius*2, ballRadius*2, sf::Color::Yellow);
	makeADisk(image);
    texture.loadFromImage(image);
	sprite.setTexture(texture);
}

sf::Vector2f Ball::getCenter()
{
	sf::Vector2f center;
	center.x = sprite.getPosition().x + ballRadius;
	center.y = sprite.getPosition().y + ballRadius;
	return center;
}
