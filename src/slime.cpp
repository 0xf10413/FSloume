#include "slime.h"

Slime::Slime (bool alignLeft) :
  MovingEntity(),
  m_eye(alignLeft),
  m_alignLeft (alignLeft),
  m_onGround (true), m_lost(false), m_victories(0),
  m_clamp()
{
  // Dessin de la forme hémi-circulaire caractéristique
  // Calcul : tout pixel distant du point au milieu (horizontalement)
  // en bas (verticalement) de plus de width/2 est transparent.
  m_image.create (SLIME_WIDTH, SLIME_HEIGHT, m_alignLeft ? sf::Color::Blue : sf::Color::Red);
  for (int i = 0; i < SLIME_WIDTH; i++)
    for (int j=0; j<SLIME_HEIGHT; j++)
      if ((i - SLIME_WIDTH/2) * (i - SLIME_WIDTH/2) +
          (j - SLIME_HEIGHT) * (j-SLIME_HEIGHT) >=
          (SLIME_WIDTH*SLIME_WIDTH/4))
        m_image.setPixel (i, j, sf::Color (0, 0, 0, 0));

  if (m_alignLeft)
  {
    // Dessin du globe oculaire (coordonnées locales du centre : (3*w/4, h/2))
    for (int i = 0; i < SLIME_WIDTH; i++)
      for (int j=0; j < SLIME_HEIGHT; j++)
        if ((i - 3*SLIME_WIDTH/4) * (i - 3*SLIME_WIDTH/4) +
            (j - SLIME_HEIGHT/2) * (j-SLIME_HEIGHT/2) <=
            EYE_RADIUS_SQUARED)
          m_image.setPixel (i, j, sf::Color::White);
  }
  else
    // Dessin du globe oculaire (coordonnées locales du centre : (w/4, h/2))
    for (int i = 0; i < SLIME_WIDTH; i++)
      for (int j=0; j < SLIME_HEIGHT; j++)
        if ((i - SLIME_WIDTH/4) * (i - SLIME_WIDTH/4) +
            (j - SLIME_HEIGHT/2) * (j-SLIME_HEIGHT/2) <=
            EYE_RADIUS_SQUARED)
          m_image.setPixel (i, j, sf::Color::White);

  m_texture.loadFromImage(m_image);
  m_sprite.setTexture (m_texture);

  if (m_alignLeft)
    m_eye.setPosition (m_x + SLIME_WIDTH/4, m_y);
  else
    m_eye.setPosition (m_x - SLIME_WIDTH/4, m_y);
}

void Slime::jump()
{
  if (!m_onGround)
    return;
  m_onGround = false;
  m_vy -= SLIME_JUMP_SPEED;
}

void Slime::prepareMove(const Input &input)
{
  /* Mise à jour des vitesses en fonction de l'input */
  if (m_alignLeft)
  {
    if (input.isKeyDown(sf::Keyboard::Up))
      jump();
    if (input.isKeyDown(sf::Keyboard::Left) && !input.isKeyDown(sf::Keyboard::Right))
      m_vx = -SLIME_HORIZONTAL_SPEED;
    else if (input.isKeyDown(sf::Keyboard::Right) && !input.isKeyDown(sf::Keyboard::Left))
      m_vx = +SLIME_HORIZONTAL_SPEED;
    else
      m_vx = 0;
  }
  else
  {
    if (input.isKeyDown(sf::Keyboard::Z))
      jump();
    if (input.isKeyDown(sf::Keyboard::Q) && !input.isKeyDown(sf::Keyboard::D))
      m_vx = -SLIME_HORIZONTAL_SPEED;
    else if (input.isKeyDown(sf::Keyboard::D) && !input.isKeyDown(sf::Keyboard::Q))
      m_vx = +SLIME_HORIZONTAL_SPEED;
    else
      m_vx = 0;
  }
}

void Slime::move(float dt, const Ball &b)
{
  /* Déplacement avec la vitesse */
  m_x += dt*m_vx;
  m_y += dt*m_vy;

  /* Perte de vitesse avec la gravité */
  if (!m_onGround)
    m_vy += GRAVITY*dt;

  /* Clamping */
  if (m_x + SLIME_WIDTH/2 > m_clamp.left + m_clamp.width)
    m_x = m_clamp.left + m_clamp.width - SLIME_WIDTH/2;

  if (m_x - SLIME_WIDTH/2 < m_clamp.left)
    m_x = m_clamp.left + SLIME_WIDTH/2;

  if (m_y + SLIME_HEIGHT/2 > m_clamp.top + m_clamp.height) // plus collision avec le sol
  {
    m_y = m_clamp.top + m_clamp.height - SLIME_HEIGHT/2;
    m_vy = 0;
    m_onGround = true;
  }

  /* Déplacement de la pupille vers la balle */
  if (m_alignLeft)
    m_eye.setPosition (m_x + SLIME_WIDTH/4, m_y);
  else
    m_eye.setPosition (m_x - SLIME_WIDTH/4, m_y);
  m_eye.lookAt (b.getPosition());

  /* Mise à jour finale du sprite */
  updateSprite();
}

void Slime::clampTo(const sf::FloatRect &rect)
{
  m_clamp = rect;
}

//void Slime::updateEye (const sf::Vector2f& b)
//{
//  sf::Vector2f newPos;
//  // Globe oculaire, de coordonnées locales (3*w/4, h/2)
//  if (left)
//      newPos = getCenter() +sf::Vector2f (SLIME_WIDTH/4,-SLIME_HEIGHT/2)
//               + ((float)sqrt (eyeRadiusSquared)-pupilRadius) * (b- (getCenter() +sf::Vector2f (SLIME_WIDTH/4,-SLIME_HEIGHT/2)))
//               /(float)sqrt (abs2 (b- (getCenter() +sf::Vector2f (SLIME_WIDTH/4,-SLIME_HEIGHT/2))))
//               - sf::Vector2f (pupilRadius,pupilRadius);
//  else
//    newPos = getCenter()+sf::Vector2f (-SLIME_WIDTH/4,-SLIME_HEIGHT/2)
//             + ((float)sqrt (eyeRadiusSquared)-pupilRadius) * (b- (getCenter() +sf::Vector2f (SLIME_WIDTH/4,-SLIME_HEIGHT/2)))
//             /(float)sqrt (abs2 (b- (getCenter() +sf::Vector2f (SLIME_WIDTH/4,-SLIME_HEIGHT/2))))
//             - sf::Vector2f (pupilRadius,pupilRadius);
//  eyeSprite.setPosition (newPos);
//}

void Slime::setX (float x)
{
  m_x = x;
  updateSprite();
}

void Slime::setY (float y)
{
  m_y = y;
  updateSprite();
}

void Slime::updateSprite()
{
  m_sprite.setPosition (
      m_x - SLIME_WIDTH/2,
      m_y - SLIME_HEIGHT/2
      );
}

void Slime::draw (sf::RenderWindow &w)
{
  w.draw(m_sprite);
  m_eye.draw(w);
}
