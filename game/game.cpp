#include "game.h"
#include "config.h"
#include "utils.h"
#include <iostream>

FGame::FGame () : sf::RenderWindow( sf::VideoMode ( WIDTH, HEIGHT ), "SFML"),
  m_event(), m_clock(), m_font(), m_input(),
  m_reinit(false),
  m_bSlime(true), m_rSlime(false), m_ball(), m_net(),
  m_menu(nullptr),
  m_game_mode(GameMode::TITLE)
{
  setFramerateLimit (60);
  m_font.loadFromFile ("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");


  sf::Color menuColor = sf::Color::Magenta;
  menuColor.a = 127;
  m_menu = new Menu(m_font, menuColor);
  sf::Vector2f margins{WIDTH/20, HEIGHT/20};
  sf::Vector2f paddings{WIDTH/40, HEIGHT/40};

  sf::Color button_color = sf::Color::Green;
  button_color.a = 127;
  m_menu->addButton ("Mode deux joueurs", button_color, margins, paddings);
  m_menu->addButton ("[Mode un joueur]", button_color, margins, paddings);
  //m_menu->addButton ("[Mode deux joueurs online]", sf::Color::Green, margins);
  //m_menu->addButton ("[Histoire]", sf::Color::Green, margins);
  m_menu->setPosition (WIDTH/2, HEIGHT/2);

  rebuildGame();
}

void FGame::rebuildGame()
{
  m_bSlime.reinit();
  m_bSlime.setX (WIDTH/4);
  m_bSlime.setY (HEIGHT);
  m_bSlime.clampTo(sf::FloatRect(0, 0, WIDTH/2 - NET_WIDTH/2, HEIGHT));

  m_rSlime.reinit();
  m_rSlime.setX (3*WIDTH/4);
  m_rSlime.setY (HEIGHT);
  m_rSlime.clampTo(sf::FloatRect(WIDTH/2 + NET_WIDTH/2, 0, WIDTH/2 - NET_WIDTH/2, HEIGHT));

  m_ball.reinit();
  m_ball.setX (WIDTH/4);
  m_ball.setY (2*HEIGHT/3);
  m_ball.clampTo(sf::FloatRect(0, 0, WIDTH, HEIGHT));

  m_net.reinit();
  m_net.setPosition (WIDTH/2, HEIGHT - NET_HEIGHT/2);

  m_reinit = false;
}

int FGame::mainLoop ()
{
  /* Réinitialiser l'horloge pour éviter de compter le temps de démarrage */
  m_clock.restart();

  // Boucle principale
  while ( isOpen() )
  {
    float eps = m_clock.restart().asSeconds();

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
      if (m_event.type == sf::Event::MouseButtonPressed && m_game_mode == GameMode::TITLE)
      {
        std::string click = m_menu->wasIClicked(m_event);
        if (click == "Mode deux joueurs")
        {
          m_game_mode = GameMode::TWO_PLAYERS;
          m_reinit = true;
        }
        if (!click.empty())
          std::cout << "Click : " << click << std::endl;
      }
    }

    if (m_reinit)
    {
      rebuildGame();
      continue;
    }

    if (m_game_mode == GameMode::TWO_PLAYERS)
    {
      m_bSlime.prepareMove(m_input);
      m_rSlime.prepareMove(m_input);
    }

    collide (eps);

    m_bSlime.pushState();
    m_rSlime.pushState();
    m_ball.pushState();

    for (int i = 0; i < BALL_ANTICIPATION; ++i)
    {
      m_ball.updatePath(i);
      collide(eps);
      m_ball.move(eps);
    }

    m_bSlime.popState();
    m_rSlime.popState();
    m_ball.popState();


    m_bSlime.move(eps, m_ball);
    m_rSlime.move(eps, m_ball);
    m_ball.move(eps);


    // Affichage
    clear();
    m_bSlime.draw(*this);
    m_rSlime.draw(*this);
    m_net.draw(*this);
    m_ball.draw(*this);

    if (m_game_mode == GameMode::TITLE)
      m_menu->draw(*this);

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

