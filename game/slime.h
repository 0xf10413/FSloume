#ifndef F_SLIME_H
#define F_SLIME_H

#include <SFML/Graphics.hpp>
#include "config.h"
#include "unique_drawable.h"
#include "eye.h"
#include "input.h"
#include "ball.h"

class Slime : public MovingEntity
{
private:
    Eye m_eye;
    bool m_alignLeft;
    bool m_onGround, m_onGround_prev;
    bool m_lost;
    bool m_main_character; // Qui a les contrôles en 1j sur android ?
    int m_victories;
    sf::FloatRect m_clamp;

    virtual void updateSprite() override;
    void jump();
public:
    Slime (bool isLeft);
    void prepareMove(const Input &input); // Enregistre les vitesses demandées
    void move(float dt, const Ball &b);

    void setMainCharacter(bool main_character);
    void toggleMainCharacter();

    /* Positionnement du centre d'inertie */
    void setX (float nx);
    void setY (float ny);
    sf::Vector2f getPosition();
    sf::Vector2f getSpeed();

    /* Contraintes */
    void clampTo (const sf::FloatRect &rect);

    virtual void draw (sf::RenderWindow &) const override;
    virtual void pushState() override;
    virtual void popState() override;
};

#endif
