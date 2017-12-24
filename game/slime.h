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
    enum class MovingHStatus {STOPPED, MOVING, MOVING_WAIT, MOVING_FAST};
    enum class MovingVStatus {STOPPED, JUMPING, JUMPING_WAIT, DOUBLE_JUMPING};
    enum Direction {LEFT, RIGHT, UP, DOWN, NONE};
    MovingHStatus m_movingh_status[2]; // Statut du déplacement à gauche et à droite
    MovingVStatus m_movingv_status, m_prev_movingv_status; // Statut du déplacement vertical
    sf::Clock m_moving_timer[3];
    int m_victories;
    sf::FloatRect m_clamp;

    virtual void updateSprite() override;
    void jump();
    void antijump();
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
    virtual void pushState() override;
    virtual void popState() override;
    virtual void draw (sf::RenderWindow &) const override;
};

#endif
