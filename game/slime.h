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
    friend class IA;
    Eye m_eye;
    bool m_alignLeft;
    bool m_lost;
    bool m_main_character; // Qui a les contrôles en 1j sur android ?
    enum MovingStatus {STOPPED, MOVING, MOVING_WAIT, MOVING_FAST};
    enum Direction {LEFT, RIGHT, NONE};
    MovingStatus m_moving_status[2]; // Statut du déplacement à gauche et à droite
    sf::Clock m_moving_timer[2];
    int m_victories;
    sf::FloatRect m_clamp;

    virtual void updateSprite() override;
    void jump();
public:
    Slime (bool isLeft);
    void prepareMove(const Input &input); // Enregistre les vitesses demandées
    void move(float dt, const Ball &b);

    bool touched(float x, float y); // A-t-on posé le doigt dessus ?
    void setMainCharacter(bool main_character);
    void toggleMainCharacter();

    /* Positionnement du centre d'inertie */
    void setX (float nx);
    void setY (float ny);
    sf::Vector2f getPosition();
    sf::Vector2f getSpeed();

    /* Contraintes */
    void clampTo (const sf::FloatRect &rect);

    virtual void reinit() override;
    virtual void draw (sf::RenderWindow &) const override;
};

#endif
