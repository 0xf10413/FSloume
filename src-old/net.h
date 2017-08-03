#ifndef F_NET_H
#define F_NET_H

#include <SFML/Graphics.hpp>
#include "main.h"

class Net
{
private:
  sf::Texture texture;
	sf::Image image;
	sf::Sprite sprite;
public:
  Net () : texture(), image(), sprite()
  {
    image.create ( netWidth, netHeight, sf::Color::Yellow );
    texture.loadFromImage(image);
    sprite.setTexture ( texture );
  }

  inline void setPosition (float x, float y) { sprite.setPosition (x, y); }
  inline const sf::Sprite &getSprite () { return sprite; }
  inline const sf::Vector2f &getPosition () { return sprite.getPosition();}
};

#endif
