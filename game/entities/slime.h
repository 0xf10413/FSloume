#ifndef F_SLIME_H
#define F_SLIME_H

#include <SFML/Graphics.hpp>
#include "config.h"
#include "tools/unique_drawable.h"
#include "eye.h"
#include "tools/input.h"
#include "ball.h"

class Slime : public MovingEntity
{
private:
    friend class IA;
    enum class MovingHStatus {STOPPED, MOVING, MOVING_WAIT, MOVING_FAST, FORCE_RETREAT};
    enum class MovingVStatus {STOPPED, JUMPING, JUMPING_WAIT, DOUBLE_JUMPING,
     FORCE_JUMPING, FAST_LAND, GROUND_POUND};
    enum Direction {LEFT, RIGHT, UP, DOWN, NONE};

    Eye m_eye;
    bool m_alignLeft;
    bool m_lost;
    bool m_main_character; // Qui a les contrôles en 1j sur android ?
    MovingHStatus m_movingh_status[2]; // Statut du déplacement à gauche et à droite
    MovingVStatus m_movingv_status, m_prev_movingv_status; // Statut du déplacement vertical
    float m_apec; // Plus haute altitude lors du dernier saut
    sf::Clock m_moving_timer[3];
    int m_victories;
    bool m_shockwave_ready; // une onde de choc en attente
    sf::FloatRect m_clamp;

    virtual void updateSprite() override;
    bool jump();
    bool doubleJump();
    bool antijump();
    bool groundPound();
    bool canGroundPound(bool tolerant);
public:
    /**
     * \param isLeft Is the slime on the left side (looking toward the right side) ?
     */
    Slime (bool isLeft);

    void forceShock();
    void prepareMove(const Input &input); // Enregistre les vitesses demandées
    void move(float dt, const Ball &b, bool fake=false);

    bool fetchShockwave(); // récupérer l'onde de choc si elle est prête
    bool lockedRetreat(); // est-on sous le coup d'une onde de choc ?

    bool touched(float x, float y); // A-t-on posé le doigt dessus ?
    void setMainCharacter(bool main_character);
    void toggleMainCharacter();

    /* Positionnement du centre d'inertie */
    void setX (float nx);
    void setY (float ny);
    void setTheta (float theta);
    sf::Vector2f getPosition();
    sf::Vector2f getSpeed();

    /* Contraintes */
    void clampTo (const sf::FloatRect &rect);
    void stopX ();

    virtual void reinit() override;
    virtual void pushState() override;
    virtual void popState() override;
    virtual void draw(sf::RenderTexture &) const override;
};

#endif
