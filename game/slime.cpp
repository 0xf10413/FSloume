#include "slime.h"

Slime::Slime (bool alignLeft) :
  MovingEntity(),
  m_eye(alignLeft),
  m_alignLeft (alignLeft),
  m_lost(false), m_main_character(false),
  m_victories(0),
  m_clamp()
{
  // Dessin de la forme hémi-circulaire caractéristique
  // Calcul : tout pixel distant du point au milieu (horizontalement)
  // en bas (verticalement) de plus de width/2 est transparent.
  m_image.create (CG::SLIME_WIDTH, CG::SLIME_HEIGHT, m_alignLeft ? sf::Color::Blue : sf::Color::Red);
  for (int i = 0; i < CG::SLIME_WIDTH; i++)
    for (int j=0; j<CG::SLIME_HEIGHT; j++)
      if ((i - CG::SLIME_WIDTH/2) * (i - CG::SLIME_WIDTH/2) +
          (j - CG::SLIME_HEIGHT) * (j-CG::SLIME_HEIGHT) >=
          (CG::SLIME_WIDTH*CG::SLIME_WIDTH/4))
        m_image.setPixel (i, j, sf::Color (0, 0, 0, 0));

  if (m_alignLeft)
  {
    // Dessin du globe oculaire (coordonnées locales du centre : (3*w/4, h/2))
    for (int i = 0; i < CG::SLIME_WIDTH; i++)
      for (int j=0; j < CG::SLIME_HEIGHT; j++)
        if ((i - 3*CG::SLIME_WIDTH/4) * (i - 3*CG::SLIME_WIDTH/4) +
            (j - CG::SLIME_HEIGHT/2) * (j-CG::SLIME_HEIGHT/2) <=
            CG::EYE_RADIUS_SQUARED)
          m_image.setPixel (i, j, sf::Color::White);
  }
  else
    // Dessin du globe oculaire (coordonnées locales du centre : (w/4, h/2))
    for (int i = 0; i < CG::SLIME_WIDTH; i++)
      for (int j=0; j < CG::SLIME_HEIGHT; j++)
        if ((i - CG::SLIME_WIDTH/4) * (i - CG::SLIME_WIDTH/4) +
            (j - CG::SLIME_HEIGHT/2) * (j-CG::SLIME_HEIGHT/2) <=
            CG::EYE_RADIUS_SQUARED)
          m_image.setPixel (i, j, sf::Color::White);

  m_texture.loadFromImage(m_image);
  m_sprite.setTexture (m_texture);

  if (m_alignLeft)
    m_eye.setPosition (m_x + CG::SLIME_WIDTH/4, m_y);
  else
    m_eye.setPosition (m_x - CG::SLIME_WIDTH/4, m_y);
}

void Slime::jump()
{
  if (!m_onGround)
    return;
  m_onGround = false;
  m_vy -= CG::SLIME_JUMP_SPEED;
}

void Slime::prepareMove(const Input &input)
{
  /* Mise à jour des vitesses en fonction de l'input */
  if (!m_alignLeft)
  {
    if (input.isKeyDown(sf::Keyboard::Up))
      jump();
    if (input.isKeyDown(sf::Keyboard::Left) && !input.isKeyDown(sf::Keyboard::Right))
      m_vx = -CG::SLIME_HORIZONTAL_SPEED;
    else if (input.isKeyDown(sf::Keyboard::Right) && !input.isKeyDown(sf::Keyboard::Left))
      m_vx = +CG::SLIME_HORIZONTAL_SPEED;
    else
      m_vx = 0;
  }
  else
  {
    if (input.isKeyDown(sf::Keyboard::Z))
      jump();
    if (input.isKeyDown(sf::Keyboard::Q) && !input.isKeyDown(sf::Keyboard::D))
      m_vx = -CG::SLIME_HORIZONTAL_SPEED;
    else if (input.isKeyDown(sf::Keyboard::D) && !input.isKeyDown(sf::Keyboard::Q))
      m_vx = +CG::SLIME_HORIZONTAL_SPEED;
    else
      m_vx = 0;

  }

  /* Version android */
  if (m_main_character && input.isTouchDown())
  {
    sf::Vector2f touchDown = input.whereIsTouch();
    if (std::abs(touchDown.x - m_x) >= 2.) // évite les vibrations
    {
      if (touchDown.y*2 > CG::HEIGHT)
      {
        if (touchDown.x < CG::WIDTH/2)
          m_vx = -CG::SLIME_HORIZONTAL_SPEED;
        else if (touchDown.x > CG::WIDTH/2)
          m_vx = +CG::SLIME_HORIZONTAL_SPEED;
      }
      else
        jump();
    }
  }
}

void Slime::move(float dt, const Ball &b)
{
  /* Déplacement avec la vitesse */
  m_x += dt*m_vx;
  m_y += dt*m_vy;

  /* Perte de vitesse avec la gravité */
  if (!m_onGround)
    m_vy += CG::GRAVITY*dt;

  /* Clamping */
  if (m_x + CG::SLIME_WIDTH/2 > m_clamp.left + m_clamp.width)
    m_x = m_clamp.left + m_clamp.width - CG::SLIME_WIDTH/2;

  if (m_x - CG::SLIME_WIDTH/2 < m_clamp.left)
    m_x = m_clamp.left + CG::SLIME_WIDTH/2;

  if (m_y > m_clamp.top + m_clamp.height) // plus collision avec le sol
  {
    m_y = m_clamp.top + m_clamp.height;
    m_vy = 0;
    m_onGround = true;
  }

  /* Déplacement de la pupille vers la balle */
  if (m_alignLeft)
    m_eye.setPosition (m_x + CG::SLIME_WIDTH/4, m_y - CG::SLIME_HEIGHT/2);
  else
    m_eye.setPosition (m_x - CG::SLIME_WIDTH/4, m_y - CG::SLIME_HEIGHT/2);
  m_eye.lookAt (b.getPosition());

  /* Mise à jour finale du sprite */
  updateSprite();
}

void Slime::clampTo(const sf::FloatRect &rect)
{
  m_clamp = rect;
}

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

sf::Vector2f Slime::getPosition()
{
  return {m_x, m_y};
}
sf::Vector2f Slime::getSpeed()
{
  return {m_vx, m_vy};
}

void Slime::updateSprite()
{
  m_sprite.setPosition (
      m_x - CG::SLIME_WIDTH/2,
      m_y - CG::SLIME_HEIGHT
      );
}

void Slime::draw (sf::RenderWindow &w) const
{
  w.draw(m_sprite);
  m_eye.draw(w);
}

void Slime::setMainCharacter(bool main_character)
{
  m_main_character = main_character;
}

void Slime::toggleMainCharacter()
{
  m_main_character = !m_main_character;
}

bool Slime::touched(float x, float y)
{
  return std::abs(m_x-x) <= CG::SLIME_WIDTH/2. &&
    m_y >= y && m_y - CG::SLIME_HEIGHT <= y;
}
