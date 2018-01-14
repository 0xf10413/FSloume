#ifndef UNIQUE_DRAWABLE_H
#define UNIQUE_DRAWABLE_H

#include <memory>
#include <SFML/Graphics.hpp>

/**
 * Une classe fréquemment utilisée. Cumule un sprite, une texture et une image.
 * Pour que l'objet soit directement dessinable, il hérite de sf::Sprite
 */
class UniqueDrawable
{
protected:
  sf::Sprite m_sprite;
  std::shared_ptr<sf::Texture> m_texture;
public:
  UniqueDrawable();
  virtual ~UniqueDrawable() = default;
  virtual void draw(sf::RenderWindow &) const;
};

class HeavyDrawable
{
protected:
  sf::Image m_image;
  sf::Texture m_texture;
  sf::Sprite m_sprite;
public:
  HeavyDrawable();
  virtual ~HeavyDrawable() = default;
  virtual void draw(sf::RenderWindow &) const;
};

class MovingEntity : public UniqueDrawable
{
private:
  std::array<float, 7> m_saved_state;
protected:
  /* Positions et vitesses du centre d'inertie */
  float m_x, m_y;
  float m_vx, m_vy;
  /* Angle de rotation et vitesse de rotation */
  float m_theta, m_omega;

  bool m_onGround; // Est au sol ?

  virtual void updateSprite() = 0; // Mise à jour du sprite post déplacement
public:
  MovingEntity();
  virtual ~MovingEntity()= default;
  virtual void reinit(); // Réinitialise la position et la vitesse
  bool getOnGround() const;

  virtual void pushState(); // Sauvegarde l'état actuelle de la position et de la vitesse
  virtual void popState(); // Restore l'état précédent de la position et de la vitesse
};

class HeavyMovingEntity : public HeavyDrawable
{
private:
  std::array<float, 7> m_saved_state;
protected:
  /* Positions et vitesses du centre d'inertie */
  float m_x, m_y;
  float m_vx, m_vy;
  /* Angle de rotation et vitesse de rotation */
  float m_theta, m_omega;

  bool m_onGround; // Est au sol ?

  virtual void updateSprite() = 0; // Mise à jour du sprite post déplacement
public:
  HeavyMovingEntity();
  virtual ~HeavyMovingEntity()= default;
  virtual void reinit(); // Réinitialise la position et la vitesse
  bool getOnGround() const;

  virtual void pushState(); // Sauvegarde l'état actuelle de la position et de la vitesse
  virtual void popState(); // Restore l'état précédent de la position et de la vitesse
};

#endif /* !UNIQUE_DRAWABLE_H */
