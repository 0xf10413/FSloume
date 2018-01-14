#include "slime.h"
#include "rc_manager.h"
#include "utils.h"

Slime::Slime (bool alignLeft) :
  MovingEntity(),
  m_eye(alignLeft),
  m_alignLeft (alignLeft),
  m_lost(false), m_main_character(false),
  m_movingh_status{MovingHStatus::STOPPED, MovingHStatus::STOPPED},
  m_movingv_status(), m_prev_movingv_status(),
  m_apec(0),
  m_moving_timer{},
  m_victories(0),
  m_shockwave_ready(false),
  m_clamp()
{
  m_texture = ResourceManager::getTexture(m_alignLeft ? "bSlime" : "rSlime");
  m_sprite.setTexture (*m_texture);
  m_sprite.setOrigin(CG::SLIME_WIDTH/2, CG::SLIME_HEIGHT/2);

  if (m_alignLeft)
    m_eye.setPosition (m_x + CG::SLIME_WIDTH/4, m_y);
  else
    m_eye.setPosition (m_x - CG::SLIME_WIDTH/4, m_y);
}

void Slime::jump()
{
  if (!m_onGround && m_movingv_status != MovingVStatus::JUMPING_WAIT)
    return;
  m_onGround = false;
  m_vy -= CG::SLIME_JUMP_SPEED;
}

/* Starts to cancel a jump */
void Slime::antijump()
{
  if (m_onGround)
    return;
  m_vy = std::max(m_vy, 0.f);
  m_vy += 2*CG::SLIME_JUMP_SPEED;
}

void Slime::groundPound()
{
  if (m_apec < CG::SLIME_HEIGHT_GROUND_POUND)
  {
    m_movingv_status = MovingVStatus::GROUND_POUND;
    m_vy = -CG::SLIME_JUMP_SPEED/2;
  }
}

void Slime::prepareMove(const Input &input)
{
  /* Mise à jour des vitesses en fonction de l'input */
  Direction dirh = Direction::NONE;
  Direction dirv = Direction::NONE;
#ifndef F_CONFIG_ANDROID
  if (!m_alignLeft)
  {
    if (input.isKeyDown(sf::Keyboard::Up) && !input.isKeyDown(sf::Keyboard::Down))
      dirv = UP;
    if (input.isKeyDown(sf::Keyboard::Down) && !input.isKeyDown(sf::Keyboard::Up))
      dirv = DOWN;
    if (input.isKeyDown(sf::Keyboard::Left) && !input.isKeyDown(sf::Keyboard::Right))
      dirh = LEFT;
    else if (input.isKeyDown(sf::Keyboard::Right) && !input.isKeyDown(sf::Keyboard::Left))
      dirh = RIGHT;
  }
  else
  {
    if (input.isKeyDown(sf::Keyboard::Z) && !input.isKeyDown(sf::Keyboard::S))
      dirv = UP;
    else if (input.isKeyDown(sf::Keyboard::S) && !input.isKeyDown(sf::Keyboard::Z))
      dirv = DOWN;
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
    /* Si aucun saut n'a été demandé, on regarde si une annulation est demandée */
    if (dirv != UP)
      for (const auto &v : touchDowns)
        if (std::abs(v.x - m_x) <= CG::SLIME_WIDTH/2 && v.y >= m_y)
        {
          dirv = DOWN;
          dirh = NONE;
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
          if (m_onGround &&
              m_moving_timer[i].getElapsedTime().asSeconds() < CG::SLIME_TIME_BOOST)
            m_movingh_status[i] = MovingHStatus::MOVING_FAST;
          break;
        case MovingHStatus::MOVING_FAST:
          break;
        case MovingHStatus::FORCE_RETREAT:
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
        case MovingHStatus::FORCE_RETREAT:
          break;
      }
  }

  /* Màj de la machine à états verticale */
  if (dirv == Direction::UP)
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
      case MovingVStatus::FORCE_JUMPING:
        break;
      case MovingVStatus::FAST_LAND:
        m_apec = m_y;
        groundPound();
        break;
      case MovingVStatus::GROUND_POUND:
        break;
    }
  else if (dirv == DOWN)
    switch(m_movingv_status)
      {
        case MovingVStatus::STOPPED:
          break;
        case MovingVStatus::JUMPING_WAIT:
        case MovingVStatus::DOUBLE_JUMPING:
        case MovingVStatus::JUMPING:
          m_movingv_status = MovingVStatus::FAST_LAND;
          m_apec = m_y;
          antijump();
          break;
        case MovingVStatus::FORCE_JUMPING:
          break;
        case MovingVStatus::FAST_LAND:
          break;
        case MovingVStatus::GROUND_POUND:
          break;
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
        case MovingVStatus::FORCE_JUMPING:
          break;
        case MovingVStatus::FAST_LAND:
          break;
        case MovingVStatus::GROUND_POUND:
          break;
      }
  /* Animation en cas de charge au sol */
  if (m_movingv_status == MovingVStatus::GROUND_POUND
      && m_apec < CG::SLIME_HEIGHT_GROUND_POUND)
  {
    float mid_height = .33*CG::HEIGHT + .66*m_apec;
    if (m_y < m_apec)
      setTheta(180*(m_y-m_apec)/(CG::HEIGHT-m_apec) *(m_alignLeft ? 1 : -1));
    else if (m_y < mid_height)
    {
      setTheta(360*(m_y-mid_height)/(mid_height-m_apec) *(m_alignLeft ? 1 : -1));
      dirh = Direction::NONE;
    }
    else
    {
      setTheta(0);
      antijump();
      dirh = Direction::NONE;
    }
  }

  /* Finalisation du mouvement horizontal */
  if (lockedRetreat())
    return;
  float dir_f = 0;
  if (dirh < Direction::UP)
  {
    dir_f = 2*dirh-1;
    if (m_movingh_status[dirh] == MovingHStatus::MOVING_FAST)
      dir_f *= 2;
  }
  m_vx = dir_f*CG::SLIME_HORIZONTAL_SPEED;

}

void Slime::move(float dt, const Ball &b, bool fake)
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
    if (!fake && m_movingv_status == MovingVStatus::GROUND_POUND)
      m_shockwave_ready = true;
    if (!fake && lockedRetreat())
        m_movingh_status[0] = m_movingh_status[1] = MovingHStatus::STOPPED;
    m_movingv_status = MovingVStatus::STOPPED;
  }

  /* Déplacement de la pupille vers la balle */
  sf::Vector2f dPupilCenter;
  if (m_alignLeft)
    dPupilCenter = rotate(sf::Vector2f(CG::SLIME_WIDTH/4, 0), m_theta*M_PI/180);
  else
    dPupilCenter = rotate(sf::Vector2f(-CG::SLIME_WIDTH/4, 0), m_theta*M_PI/180);
  m_eye.setPosition (m_x + dPupilCenter.x, m_y - CG::SLIME_HEIGHT/2 + dPupilCenter.y);
  m_eye.lookAt (b.getPosition());

  /* Mise à jour finale du sprite */
  updateSprite();
}

bool Slime::lockedRetreat()
{
  return m_movingh_status[0] == MovingHStatus::FORCE_RETREAT ||
    m_movingh_status[1] == MovingHStatus::FORCE_RETREAT;
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

void Slime::setTheta (float theta)
{
  m_theta = theta;
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
      m_x,
      m_y-CG::SLIME_HEIGHT/2
      );
  m_sprite.setRotation(m_theta);
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

void Slime::stopX ()
{
  m_vx = 0;
}

void Slime::forceShock()
{
  m_vy = -.5*CG::SLIME_JUMP_SPEED;
  m_vx = m_alignLeft ? -CG::SLIME_HORIZONTAL_SPEED : CG::SLIME_HORIZONTAL_SPEED;
  m_movingv_status = MovingVStatus::FORCE_JUMPING;
  m_movingh_status[0] = MovingHStatus::FORCE_RETREAT;
  m_movingh_status[1] = MovingHStatus::FORCE_RETREAT;
  m_onGround = false;
}

bool Slime::fetchShockwave()
{
  if (m_shockwave_ready)
  {
    m_shockwave_ready = false;
    return true;
  }
  return false;
}
