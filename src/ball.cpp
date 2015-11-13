#include "ball.h"

Ball::Ball()
{
	x = y = vx = vy = 0;
	image.Create(ballRadius*2, ballRadius*2, sf::Color::Yellow);
	makeADisk(image);
	sprite.SetImage(image);
}

sf::Vector2f Ball::getCenter()
{
	sf::Vector2f center;
	center.x = sprite.GetPosition().x + ballRadius;
	center.y = sprite.GetPosition().y + ballRadius;
	return center;
}
