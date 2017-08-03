#ifndef F_SLIME_H
#define F_SLIME_H

#include <SFML/Graphics.hpp>
#include "main.h"
#include "unique_drawable.h"
#include "eye.h"
#include "input.h"

class Slime : public MovingEntity
{
private:
  Eye m_eye;
	bool m_alignLeft;
	bool m_onGround;
  bool m_lost;
	int m_victories;
  sf::Rect<float> m_clamp;

  void updateSprite(); // Met à jour le sprite après changement du centre d'inertie
  void jump();
public:
	Slime (bool isLeft);
  void move(const Input &input, float dt);

  /* Positionnement du centre d'inertie */
	void setX (float nx);
	void setY (float ny);

  /* Contraintes */
  void clampTo (const sf::Rect<float> &rect);

	//void updateEye (const sf::Vector2f &); // Reçoit le centre de la balle
};

#endif
