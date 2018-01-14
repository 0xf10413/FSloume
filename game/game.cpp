#include "game.h"
#include "config.h"
#include "utils.h"
#include "ia.h"
#include <iostream>
#include <cmath>
#include <cassert>

FGame::FGame () :
  sf::RenderWindow(sf::VideoMode (CG::WIDTH, CG::HEIGHT ), "FSloume"),
  m_event(), m_clock(),
  m_font(), m_input(),
  m_reinit(false), m_full_reinit(false),
  m_paused(false), m_defer_unlock_pause(false),
  m_background(),
  m_bSlime(true), m_rSlime(false), m_ball(), m_net(),
  m_main_menu(nullptr), m_pause_menu(nullptr),
  m_game_mode(GameMode::TITLE), m_branch_mode(BranchMode::PLAYING),
  m_game_over_countdown(),
  m_lScore(0, true),
  m_rScore(0, false),
  m_gameOverText(),
  m_targets{},
  m_dangerpt(),
  m_shockwaves{true,false},
  m_pgenerator(30, "star")
{
  setFramerateLimit (60);
  m_font = ResourceManager::getFont("8bitoperator");

  m_game_mode = GameMode::TITLE; // Changer à TITLE pour afficher le menu
  m_gameOverText.setFont(*m_font);
  m_gameOverText.setCharacterSize(CG::FONT_BASE_SIZE_PX);
  m_gameOverText.setString("YOU failed!");
  m_gameOverText.setPosition(
      CG::WIDTH/2 - m_gameOverText.getGlobalBounds().left - m_gameOverText.getGlobalBounds().width/2,
      CG::HEIGHT/2 - m_gameOverText.getGlobalBounds().top - m_gameOverText.getGlobalBounds().height/2
      );
  m_gameOverText.setFillColor(sf::Color::White);

  m_main_menu = new Menu(*m_font, CG::MENU_BG_COLOR);
  m_pause_menu = new Menu(*m_font, CG::MENU_BG_COLOR);
  sf::Vector2f margins{(float)CG::WIDTH/20, (float)CG::HEIGHT/20};
  sf::Vector2f paddings{(float)CG::WIDTH/40, (float)CG::HEIGHT/40};

  m_rSlime.setMainCharacter(false);
  m_bSlime.setMainCharacter(true);

  m_lScore.setFont(m_font.get());
  m_rScore.setFont(m_font.get());

  sf::Color button_color = sf::Color::Green;
  button_color.a = 127;
#ifndef F_CONFIG_ANDROID
  m_main_menu->addButton ("Mode deux joueurs", button_color, margins, paddings);
#endif
  m_main_menu->addButton ("Mode un joueur", button_color, margins, paddings);
  m_main_menu->addButton ("Test", button_color, margins, paddings);
  //m_menu->addButton ("[Mode deux joueurs online]", sf::Color::Green, margins);
  //m_menu->addButton ("[Histoire]", sf::Color::Green, margins);
  m_main_menu->setPosition (CG::WIDTH/2, CG::HEIGHT/2);

  m_pause_menu->addButton ("Reprendre", button_color, margins, paddings);
  m_pause_menu->addButton ("Recommencer", button_color, margins, paddings);
  m_pause_menu->addButton ("Menu principal", button_color, margins, paddings);
  m_pause_menu->setPosition (CG::WIDTH/2, CG::HEIGHT/2);

  rebuildGame();
}

/* Replace tous les éléments à leurs positions initiales */
void FGame::rebuildGame()
{
  m_bSlime.reinit();
  m_bSlime.setX (CG::WIDTH/4);
  m_bSlime.setY (CG::HEIGHT);
  m_bSlime.clampTo(sf::FloatRect(0, 0, CG::WIDTH/2 - CG::NET_WIDTH/2, CG::HEIGHT));

  m_rSlime.reinit();
  m_rSlime.setX (3*CG::WIDTH/4);
  m_rSlime.setY (CG::HEIGHT);
  m_rSlime.clampTo(sf::FloatRect(CG::WIDTH/2 + CG::NET_WIDTH/2, 0, CG::WIDTH/2 - CG::NET_WIDTH/2, CG::HEIGHT));

  m_ball.reinit();
  if (m_branch_mode == BranchMode::BLUE_LOST)
    m_ball.setX(3*CG::WIDTH/4);
  else
    m_ball.setX (CG::WIDTH/4);
  m_ball.setY (2*CG::HEIGHT/3);
  m_ball.clampTo(sf::FloatRect(0, 0, CG::WIDTH, CG::HEIGHT));

  m_net.reinit();
  m_net.setPosition (CG::WIDTH/2, CG::HEIGHT - CG::NET_HEIGHT/2);

  m_lScore.setPosition(0,0);
  m_rScore.setPosition(CG::WIDTH,0);
  if (m_full_reinit)
  {
    m_lScore.reinit();
    m_rScore.reinit();
    m_full_reinit = false;
  }

  m_branch_mode = BranchMode::PLAYING;

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
        {
          if (m_paused)
            m_paused = false;
          else if (m_game_mode != GameMode::TITLE)
            m_paused = true;
          else
          {
            setActive(false);
            close();
          }
        }
        else if (m_event.key.code == sf::Keyboard::A)
          m_shockwaves[0].setPosition(CG::WIDTH/4, CG::HEIGHT);
        else if (m_event.key.code == sf::Keyboard::E)
          m_shockwaves[1].setPosition(3*CG::WIDTH/4, CG::HEIGHT);
      }
      if (m_event.type == sf::Event::LostFocus)
      {
        m_paused = true;
        setActive(false);
      }
      if (m_event.type == sf::Event::GainedFocus)
      {
        if (m_game_mode == GameMode::TITLE)
          m_defer_unlock_pause = true;
        setActive(true);
      }
      if ((m_event.type == sf::Event::MouseButtonPressed ||
            m_event.type == sf::Event::TouchEnded) && m_game_mode == GameMode::TITLE)
      {
        std::string click = m_main_menu->wasIClicked(m_event);
        if (click == "Mode deux joueurs")
        {
          m_game_mode = GameMode::TWO_PLAYERS;
          m_reinit = m_full_reinit = true;
        }
        if (click == "Mode un joueur")
        {
          m_game_mode = GameMode::ONE_PLAYER;
          m_reinit = m_full_reinit = true;
        }
        if (click == "Test")
        {
          m_game_mode = GameMode::TEST;
          m_reinit = m_full_reinit = true;
        }
      }
      if ((m_event.type == sf::Event::MouseButtonPressed ||
            m_event.type == sf::Event::TouchEnded) && m_paused)
      {
        std::string click = m_pause_menu->wasIClicked(m_event);
        if (click == "Reprendre")
        {
          m_paused = false;
        }
        if (click == "Recommencer")
        {
          m_paused = false;
          m_reinit = true;
          m_branch_mode = RED_LOST;
          m_lScore.reinit();
          m_rScore.reinit();
        }
        if (click == "Menu principal")
        {
          m_paused = false;
          m_game_mode = GameMode::TITLE;
          m_reinit = m_full_reinit = true;
        }
      }
    }

    if (m_reinit)
    {
      rebuildGame();
      continue;
    }

    /* Branche principale de jeu */
    if (m_branch_mode == BranchMode::PLAYING && !m_paused)
    {
      if (m_game_mode == GameMode::TWO_PLAYERS || m_game_mode == GameMode::ONE_PLAYER
        || m_game_mode == GameMode::TEST)
      {
        m_bSlime.prepareMove(m_input);
        m_rSlime.prepareMove(m_input);
      }


      m_bSlime.pushState();
      m_rSlime.pushState();
      m_ball.pushState();

      if (m_game_mode == GameMode::TITLE)
      {
        m_bSlime.stopX();
        m_rSlime.stopX();
      }

      m_dangerpt.setPosition({-100, -100});
      for (int i = 0; i < CG::BALL_ANTICIPATION; ++i)
      {
        m_ball.updatePath(i);
        if (!m_ball.getOnGround() || m_game_mode == GameMode::TEST)
        {
          collide(eps, true);
          m_ball.move(eps);
          //IA(IA::Difficulty::TOO_EASY).interact(m_bSlime, m_ball, m_dangerpt.getPosition());
          //IA(IA::Difficulty::TOO_EASY).interact(m_rSlime, m_ball, m_dangerpt.getPosition());
          m_bSlime.move(eps, m_ball, true);
          m_rSlime.move(eps, m_ball, true);
        }
        else
        {
          m_dangerpt.setPosition(m_ball.getPosition());
        }
      }

      m_bSlime.popState();
      m_rSlime.popState();
      m_ball.popState();

      /* Action IA */
      if (m_game_mode == GameMode::TITLE)
      {
        IA(IA::Difficulty::TOO_EASY).interact(m_bSlime, m_ball, m_dangerpt.getPosition(),
            m_shockwaves[1]);
        IA(IA::Difficulty::TOO_EASY).interact(m_rSlime, m_ball, m_dangerpt.getPosition(),
            m_shockwaves[0]);
      }
      if (m_game_mode == GameMode::ONE_PLAYER)
      {
        IA(IA::Difficulty::TOO_EASY).interact(m_rSlime, m_ball, m_dangerpt.getPosition(),
            m_shockwaves[0]);
      }


      collide (eps);

      m_ball.move(eps);
      m_bSlime.move(eps, m_ball);
      m_rSlime.move(eps, m_ball);
      if (m_bSlime.fetchShockwave())
        m_shockwaves[0].setPosition(m_bSlime.getPosition().x, m_bSlime.getPosition().y);
      m_shockwaves[0].move(eps);
      if (m_rSlime.fetchShockwave())
        m_shockwaves[1].setPosition(m_rSlime.getPosition().x, m_rSlime.getPosition().y);
      m_shockwaves[1].move(eps);



      if (m_ball.getOnGround() && m_game_mode != GameMode::TEST) // Game over ! Mais pour qui ?
      {
        if (m_ball.getPosition().x > CG::WIDTH/2)
        {
          m_branch_mode = BranchMode::RED_LOST;
          m_gameOverText.setFillColor(sf::Color::Red);
          if (m_game_mode != GameMode::TITLE)
            ++m_lScore;
        }
        else
        {
          m_branch_mode = BranchMode::BLUE_LOST;
          m_gameOverText.setFillColor(sf::Color::Blue);
          if (m_game_mode != GameMode::TITLE)
            ++m_rScore;
        }
        m_game_over_countdown = sf::milliseconds(1000);
      }

    }

    /* Branche de Game Over */
    if (m_branch_mode == BranchMode::RED_LOST || m_branch_mode == BranchMode::BLUE_LOST)
    {
      m_game_over_countdown -= sf::seconds(eps);
      if (m_game_over_countdown <= sf::Time::Zero)
        m_reinit = true;
    }

    /* Animation des éléments hors pause */
    if (!m_paused)
    {
      m_background.animate(eps);
      m_pgenerator.animate(eps);
    }

    /* Placement des mires */
    const std::vector<sf::Vector2f> touches = m_input.whereAreTouch();
    assert(touches.size() <= m_targets.size());
    for (size_t i = 0; i < touches.size(); ++i)
      m_targets[i].setPosition(touches[i].x, touches[i].y);
    for (size_t i = touches.size(); i < m_targets.size(); ++i)
      m_targets[i].hide();



    // Affichage
    clear();
    m_background.draw(*this);
    m_bSlime.draw(*this);
    m_rSlime.draw(*this);
    m_net.draw(*this);
    m_ball.draw(*this);
    m_shockwaves[0].draw(*this);
    m_shockwaves[1].draw(*this);
    draw(m_pgenerator);

    if (m_branch_mode != BranchMode::PLAYING)
      draw(m_gameOverText);

    if (m_game_mode == GameMode::TITLE)
      m_main_menu->draw(*this);
    else
    {
      m_lScore.draw(*this);
      m_rScore.draw(*this);
    }

    if (m_defer_unlock_pause)
      m_defer_unlock_pause = m_paused = false;
    if (m_paused && (m_game_mode != GameMode::TITLE))
      m_pause_menu->draw(*this);

    for (auto &target : m_targets)
      target.draw(*this);
    //if (m_game_mode != GameMode::TEST)
        m_dangerpt.draw(*this);

    display();
  }
  return EXIT_SUCCESS;
}

void FGame::collide (float dt, bool fake)
{
  /* À l'appel de cette fonction, tous les vecteurs vitesse sont déjà calculés */
  /* On observe juste les collisions qui se produiraient si les objets se déplacaient */
  /* On suppose qu'entre t et t+dt, les objets ont un mouvement rectiligne uniforme */
  /* (décrit par leurs vitesses à l'instant t) */

  // TODO : gérer les collisions type clamp ici aussi

  /* rSloume et shockwave b */
  if (std::abs(m_shockwaves[0].getPosition().x - m_rSlime.getPosition().x)
      < CG::SHOCKWAVE_WIDTH/2 + CG::SLIME_WIDTH/2 &&
      std::abs(m_rSlime.getPosition().y - m_shockwaves[0].getPosition().y)
        < CG::SHOCKWAVE_HEIGHT && !fake)
    m_rSlime.forceShock();

  /* bSloume et shockwave r */
  if (std::abs(m_shockwaves[1].getPosition().x - m_bSlime.getPosition().x)
      < CG::SHOCKWAVE_WIDTH/2 + CG::SLIME_WIDTH/2 &&
      std::abs(m_bSlime.getPosition().y - m_shockwaves[1].getPosition().y)
        < CG::SHOCKWAVE_HEIGHT && !fake)
    m_bSlime.forceShock();

  /* balle et shockwaves */
  for (int i = 0; i < 2; ++i)
    if (std::abs(m_shockwaves[i].getPosition().x - m_ball.getPosition().x)
        < CG::SHOCKWAVE_WIDTH/2 + CG::BALL_RADIUS &&
        std::abs(m_shockwaves[i].getPosition().y - m_ball.getPosition().y)
        < CG::SHOCKWAVE_HEIGHT && !fake)
      m_ball.forceBounce();

  /* bSlime et balle */
  float t = collideTwoCircles(
      m_bSlime.getPosition(), CG::SLIME_HEIGHT, m_bSlime.getSpeed(),
      m_ball.getPosition(), CG::BALL_RADIUS, m_ball.getSpeed(), dt);
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
    v2p *= CG::BALL_ELASTICITY*CG::SLIME_ELASTICITY;
    m_ball.setSpeed(v2p);

    if (!fake)
    {
      m_pgenerator.setPosition(x2.x, x2.y);
      m_pgenerator.pulse(norm2(v2)/200*deltaX, sf::Color::White);
    }
  }

  /* rSlime et balle */
  t = collideTwoCircles(
      m_rSlime.getPosition(), CG::SLIME_HEIGHT, m_rSlime.getSpeed(),
      m_ball.getPosition(), CG::BALL_RADIUS, m_ball.getSpeed(), dt);
  if (t >= 0.) // Intersection !
  {
    sf::Vector2f v1 = m_rSlime.getSpeed();
    sf::Vector2f x1 = m_rSlime.getPosition() + t*v1;
    sf::Vector2f v2 = m_ball.getSpeed();
    sf::Vector2f x2 = m_ball.getPosition() + t*v2;

    sf::Vector2f deltaX = x2 - x1;
    sf::Vector2f v2p = v2 - 2*dotProduct(v2 - v1, deltaX)
      *deltaX/abs2(deltaX);
    v2p *= CG::BALL_ELASTICITY*CG::SLIME_ELASTICITY;
    m_ball.setSpeed(v2p);

    if (!fake)
    {
      m_pgenerator.setPosition(x2.x, x2.y);
      m_pgenerator.pulse(norm2(v2)/200*deltaX, sf::Color::White);
    }
  }


  /* Protection contre le crush */
  m_ball.setSpeed(uncrushCircleFromRecAndCircle(m_ball.getPosition(), CG::BALL_RADIUS,
        m_ball.getSpeed(), m_bSlime.getPosition(), CG::SLIME_WIDTH/2));
  m_ball.setSpeed(uncrushCircleFromRecAndCircle(m_ball.getPosition(), CG::BALL_RADIUS,
        m_ball.getSpeed(), m_rSlime.getPosition(), CG::SLIME_WIDTH/2));

  /* filet et balle */
  sf::Vector2f bounce = collideWithFixRectangle(
      m_net.getBox(),
      m_ball.getPosition(), CG::BALL_RADIUS,m_ball.getSpeed(),  dt
      );

  if (abs2(bounce) != 0.)
  {
    auto speed = m_ball.getSpeed();
    if (bounce.x != 0.)
      speed.x = copysignf(speed.x, bounce.x);
    if (bounce.y != 0.)
      speed.y = copysignf(speed.y, bounce.y);
    m_ball.setSpeed(speed);
  }
}

FGame::~FGame()
{
  delete m_main_menu;
  delete m_pause_menu;
}
