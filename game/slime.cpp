#include "slime.h"

Slime::Slime (bool alignLeft) :
  MovingEntity(),
  m_eye(alignLeft),
  m_alignLeft (alignLeft),
  m_lost(false), m_main_character(false),
  m_movingh_status{MovingHStatus::STOPPED, MovingHStatus::STOPPED},
  m_movingv_status(), m_prev_movingv_status(),
  m_moving_timer{},
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
  if (!m_alignLeft && !m_onGround) // TODO: corriger la dissymétrie
    return;
  m_onGround = false;
  m_vy -= CG::SLIME_JUMP_SPEED;
}

void Slime::prepareMove(const Input &input)
{
  /* Mise à jour des vitesses en fonction de l'input */
  Direction dirh = Direction::NONE;
  Direction dirv = Direction::NONE;
#ifndef F_CONFIG_ANDROID
  if (!m_alignLeft)
  {
    if (input.isKeyDown(sf::Keyboard::Up))
      dirv = Direction::UP;
    if (input.isKeyDown(sf::Keyboard::Left) && !input.isKeyDown(sf::Keyboard::Right))
      dirh = LEFT;
    else if (input.isKeyDown(sf::Keyboard::Right) && !input.isKeyDown(sf::Keyboard::Left))
      dirh = RIGHT;
  }
  else
  {
    if (input.isKeyDown(sf::Keyboard::Z))
      dirv = UP;
    if (input.isKeyDown(sf::Keyboard::Q) && !input.isKeyDown(sf::Keyboard::D))
      dirh = LEFT;
    else if (input.isKeyDown(sf::Keyboard::D) && !input.isKeyDown(sf::Keyboard::Q))
      dirh = RIGHT;
  }
#else
  /* Version android */
  if (m_main_character && input.isTouchDown())
  {
    std::vector<sf::Vector2f> touchDowns = input.whereAreTouch();
    for (const auto &v : touchDowns)
    {
      if (std::abs(v.x - m_x) >= 2.) // évite les vibrations
      {
        if (v.y*2 > CG::HEIGHT)
        {
          if (v.x < CG::WIDTH/2)
            dirh = LEFT;
          else if (v.x > CG::WIDTH/2)
            dirh = RIGHT;
        }
        else
          dirv = UP;
      }
    }
  }
#endif

  /* Màj de la machine à états horizontale */
  for (int i = 0; i < 2; ++i)
  {
    if (dirh == i)
      switch (m_movingh_status[i])
      {
        case MovingHStatus::STOPPED:
          m_movingh_status[i] = MovingHStatus::MOVING;
          break;
        case MovingHStatus::MOVING:
          break;
        case MovingHStatus::MOVING_WAIT:
          if (m_moving_timer[i].getElapsedTime().asSeconds() < CG::SLIME_TIME_BOOST)
            m_movingh_status[i] = MovingHStatus::MOVING_FAST;
          break;
        case MovingHStatus::MOVING_FAST:
          break;
      }
    else
      switch (m_movingh_status[i])
      {
        case MovingHStatus::STOPPED:
          break;
        case MovingHStatus::MOVING:
          m_movingh_status[i] = MovingHStatus::MOVING_WAIT;
          m_moving_timer[i].restart();
          break;
        case MovingHStatus::MOVING_WAIT:
          if (m_moving_timer[i].getElapsedTime().asSeconds() >= CG::SLIME_TIME_BOOST)
            m_movingh_status[i] = MovingHStatus::STOPPED;
          break;
        case MovingHStatus::MOVING_FAST:
          m_movingh_status[i] = MovingHStatus::STOPPED;
          break;
      }
  }

  /* Màj de la machine à états verticale */
  if (dirv == Direction::UP)
  {
    switch(m_movingv_status)
    {
      case MovingVStatus::STOPPED:
        jump();
        m_movingv_status = MovingVStatus::JUMPING;
        m_moving_timer[2].restart();
        break;
      case MovingVStatus::JUMPING:
        break;
      case MovingVStatus::JUMPING_WAIT:
        if (m_moving_timer[2].getElapsedTime().asSeconds() > CG::SLIME_DOUBLE_JUMP_TIME)
        {
          jump();
          m_movingv_status = MovingVStatus::DOUBLE_JUMPING;
        }
        break;
      case MovingVStatus::DOUBLE_JUMPING:
        break;
    }
  }
  else
    switch(m_movingv_status)
      {
        case MovingVStatus::STOPPED:
          break;
        case MovingVStatus::JUMPING:
          m_movingv_status = MovingVStatus::JUMPING_WAIT;
          break;
        case MovingVStatus::JUMPING_WAIT:
          break;
        case MovingVStatus::DOUBLE_JUMPING:
          break;
      }

  /* Finalisation du mouvement horizontal */
  float dir_f = 0;
  if (dirh < Direction::UP)
  {
    dir_f = 2*dirh-1;
    if (m_movingh_status[dirh] == MovingHStatus::MOVING_FAST)
      dir_f *= 2;
  }
  m_vx = dir_f*CG::SLIME_HORIZONTAL_SPEED;

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
    m_movingv_status = MovingVStatus::STOPPED;
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

void Slime::pushState()
{
  MovingEntity::pushState();
  m_prev_movingv_status = m_movingv_status;
}

void Slime::popState()
{
  MovingEntity::popState();
  m_movingv_status = m_prev_movingv_status;
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

void Slime::reinit()
{
  m_movingh_status[0] = MovingHStatus::STOPPED;
  m_movingh_status[1] = MovingHStatus::STOPPED;
  m_movingv_status = MovingVStatus::STOPPED;
  MovingEntity::reinit();
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
