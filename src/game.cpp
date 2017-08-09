#include "game.h"
#include "main.h"
#include "utils.h"
#include <iostream>
#include <cassert>

FGame::FGame () : sf::RenderWindow( sf::VideoMode ( WIDTH, HEIGHT ), "SFML"),
  m_event(), m_clock(), m_font(), m_input(),
  m_reinit(false),
  m_bSlime(true), m_rSlime(false), m_ball(), m_net()
{
  setFramerateLimit (60);
  m_font.loadFromFile ("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

  m_bSlime.setX (WIDTH/4);
  m_bSlime.setY (HEIGHT);
  m_bSlime.clampTo(sf::FloatRect(0, 0, WIDTH/2 - NET_WIDTH/2, HEIGHT));

  m_rSlime.setX (3*WIDTH/4);
  m_rSlime.setY (HEIGHT);
  m_rSlime.clampTo(sf::FloatRect(WIDTH/2 + NET_WIDTH/2, 0, WIDTH/2 - NET_WIDTH/2, HEIGHT));

  m_ball.setX (WIDTH/4);
  m_ball.setY (2*HEIGHT/3);
  m_ball.clampTo(sf::FloatRect(0, 0, WIDTH, HEIGHT));

  m_net.setPosition (WIDTH/2, HEIGHT - NET_HEIGHT/2);

  m_reinit = false;
}

int FGame::mainLoop ()
{
  // Boucle principale
  while ( isOpen() )
  {
    float eps = m_clock.restart().asSeconds();
    // Réinitialisation de la partie
    if (m_reinit)
    {
      //m_bSlime.setX ( width/4-slimeWidth/2 );
      //m_bSlime.setY ( height-slimeHeight );

      //m_rSlime.setX ( 3*width/4-slimeWidth/2 );
      //m_rSlime.setY ( height-slimeHeight );

      //if (m_rSlime.lost)
      //  ball.setX ( m_bSlime.x+slimeWidth/2-ballRadius );
      //else
      //  ball.setX ( m_rSlime.x+slimeWidth/2-ballRadius );

      //ball.setY ( height/2 );

      //ball.vx = ball.vy = 0;
      //m_bSlime.vx = bSlime.vy = 0;
      //m_rSlime.vx = rSlime.vy = 0;

      //m_reinit = m_bSlime.lost = m_rSlime.lost = false;
    }

    //if (m_bSlime.lost || m_rSlime.lost) // Quelqu'un a perdu ?
    //{
    //  sf::Text text ("Perdu !", font);
    //  if (m_bSlime.lost)
    //    text.setFillColor (sf::Color::Blue);
    //  else
    //    text.setFillColor (sf::Color::Red);

    //  draw (text);
    //  display ();
    //  if (m_clock.getElapsedTime().asSeconds() < 1./60.)
    //    continue;

    //  m_reinit = true;
    //  continue;
    //}

    while ( pollEvent ( m_event ) )
    {
      m_input.process(m_event);

      if ( m_event.type == sf::Event::Closed )
        close();
      if ( m_event.type == sf::Event::KeyPressed )
      {
        if ( m_event.key.code == sf::Keyboard::Escape )
          close();
      }
    }

    m_bSlime.prepareMove(m_input);
    m_rSlime.prepareMove(m_input);

    collide (eps);

    m_bSlime.move(eps, m_ball);
    m_rSlime.move(eps, m_ball);
    m_ball.move(eps);


    // Affichage
    clear();
    m_bSlime.draw(*this);
    m_rSlime.draw(*this);
    m_net.draw(*this);
    m_ball.draw(*this);
    display();
  }
  return EXIT_SUCCESS;
}

void FGame::collide (float dt)
{
  /* À l'appel de cette fonction, tous les vecteurs vitesse sont déjà calculés */
  /* On observe juste les collisions qui se produiraient si les objets se déplacaient */
  /* On suppose qu'entre t et t+dt, les objets ont un mouvement rectiligne uniforme */
  /* (décrit par leurs vitesses à l'instant t) */

  /* bSlime et balle */
  float t = collideTwoCircles(
      m_bSlime.getPosition(), SLIME_HEIGHT, m_bSlime.getSpeed(),
      m_ball.getPosition(), BALL_RADIUS, m_ball.getSpeed(), dt);
  if (t >= 0.) // Intersection !
  {
    /* cf. https://en.wikipedia.org/wiki/Elastic_collision */
    /* La masse du sloume est très élevée */
    sf::Vector2f v1 = m_bSlime.getSpeed();
    sf::Vector2f x1 = m_bSlime.getPosition() + t*v1;
    sf::Vector2f v2 = m_ball.getSpeed();
    sf::Vector2f x2 = m_ball.getPosition() + t*v2;

    sf::Vector2f deltaX = x2 - x1;
    sf::Vector2f v2p = v2 - 2*dotProduct(v2 - v1, deltaX)
      *deltaX/abs2(deltaX);
    v2p *= BALL_ELASTICITY*SLIME_ELASTICITY;
    m_ball.setSpeed(v2p);
  }

  /* rSlime et balle */
  t = collideTwoCircles(
      m_rSlime.getPosition(), SLIME_HEIGHT, m_rSlime.getSpeed(),
      m_ball.getPosition(), BALL_RADIUS, m_ball.getSpeed(), dt);
  if (t >= 0.) // Intersection !
  {
    sf::Vector2f v1 = m_rSlime.getSpeed();
    sf::Vector2f x1 = m_rSlime.getPosition() + t*v1;
    sf::Vector2f v2 = m_ball.getSpeed();
    sf::Vector2f x2 = m_ball.getPosition() + t*v2;

    sf::Vector2f deltaX = x2 - x1;
    sf::Vector2f v2p = v2 - 2*dotProduct(v2 - v1, deltaX)
      *deltaX/abs2(deltaX);
    v2p *= BALL_ELASTICITY*SLIME_ELASTICITY;
    m_ball.setSpeed(v2p);
  }


  /* filet et balle */
  sf::Vector2f bounce = collideWithFixRectangle(
      m_net.getBox(),
      m_ball.getPosition(), BALL_RADIUS,m_ball.getSpeed(),  dt
      );

  if (abs2(bounce) != 0.)
  {
    auto speed = m_ball.getSpeed();
    if (bounce.x != 0.)
      speed.x = std::copysignf(speed.x, bounce.x);
    if (bounce.y != 0.)
      speed.y = std::copysignf(speed.y, bounce.y);
    m_ball.setSpeed(speed);
  }
}

