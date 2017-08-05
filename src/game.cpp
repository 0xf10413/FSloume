#include "game.h"
#include "main.h"
#include <iostream>

FGame::FGame () : sf::RenderWindow( sf::VideoMode ( WIDTH, HEIGHT ), "SFML"),
  m_event(), m_clock(), m_font(), m_input(),
  m_reinit(false),
  m_bSlime(true), m_rSlime(false), m_ball()//, net()
{
  setFramerateLimit (60);
  m_font.loadFromFile ("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");

  m_bSlime.setX (WIDTH/4);
  m_bSlime.setY (HEIGHT);
  m_bSlime.clampTo(sf::FloatRect(0, 0, WIDTH/2, HEIGHT));

  m_rSlime.setX (3*WIDTH/4);
  m_rSlime.setY (HEIGHT);
  m_rSlime.clampTo(sf::FloatRect(WIDTH/2, 0, WIDTH/2, HEIGHT));

  m_ball.setX (WIDTH/4);
  m_ball.setY (2*HEIGHT/3);
  m_ball.clampTo(sf::FloatRect(0, 0, WIDTH, HEIGHT));

  //net.setPosition ( ( width-netWidth ) /2,  height-netHeight );


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
    m_ball.draw(*this);
    //draw (net.getSprite() );
    display();
  }
  return EXIT_SUCCESS;
}

/* Intersection entre deux cercles mouvants. Nomenclature :
 * Premier cercle : centre M, rayon R, vitesse V
 * Second cercle : centre M' (noté Mp), rayon R' (noté Rp), vitesse V (Vp)
 * pour un temps allant de 0 à dt
 * Renvoie le temps de collision s'il existe, -1 sinon
 **/
float collideTwoCircles (sf::Vector2f M, float R, sf::Vector2f V,
    sf::Vector2f Mp, float Rp, sf::Vector2f Vp, float dt)
{
  /* On montre que l'existence d'un temps d'intersection équivaut à
   * trouver la plus petite racine d'un trinôme */
  sf::Vector2f delta_v = Vp - V;
  float RppR2 = pow2(R+Rp);
  sf::Vector2f MpM = Mp - M;
  float a = abs2(delta_v);
  float b = dotProduct(MpM, delta_v);
  float c = abs2(MpM) - RppR2;

  if (a == 0.) // Pas de trinôme
  {
    if (b == 0.) // Pas de solution
      return -1;
    float t = -c/b;
    if (t < 0. || t > dt) // Hors de l'intervalle considéré
      return -1;
    return -c/b;
  }

  float delta = b*b - 4*a*c;
  if (delta < 0) // Pas de solution
    return -1.;

  float t_min = (-b - std::sqrt(delta)) / (2*a);
  float t_max = (-b + std::sqrt(delta)) / (2*a);
  if (t_min > dt || t_max < 0.) // Hors de l'intervalle considéré
    return -1;
  return std::max(0.f, t_min);
}

void FGame::collide (float dt)
{
  /* À l'appel de cette fonction, tous les vecteurs vitesse sont déjà calculés */
  /* On observe juste les collisions qui se produiraient si les objets se déplacaient */
  /* On suppose qu'entre t et t+dt, les objets ont un mouvement rectiligne uniforme */
  /* (décrit par leurs vitesses à l'instant t) */
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

  /* m_bSlime-balle */
  //if ( abs2 ( m_bSlime.getCenter()-ball.getCenter() ) <= pow2 ( ballRadius+slimeWidth/2 ) )
  //{
  //  sf::Vector2f newV = symetric(sf::Vector2f(ball.vx, ball.vy), ball.getCenter()-m_bSlime.getCenter());
  //  ball.vx = -newV.x+m_bSlime.vx;
  //  ball.vy = -newV.y+m_bSlime.vy;
  //  sf::Vector2f newPos = (slimeHeight+ballRadius)/(float)sqrt(abs2(ball.getCenter() - m_bSlime.getCenter()))
  //    *(ball.getCenter() - m_bSlime.getCenter()) + bSlime.getCenter()-sf::Vector2f(ballRadius,ballRadius);
  //  ball.setX(newPos.x);
  //  ball.setY(newPos.y);
  //}

  //// m_bSlime-sol
  //if ( m_bSlime.y  > height- slimeHeight )
  //{
  //  m_bSlime.onGround = true;
  //  m_bSlime.vy = 0;
  //  m_bSlime.setY ( height-slimeHeight );
  //}
  //// m_bSlime-mur gauche
  //if ( m_bSlime.x < 0)
  //{
  //  m_bSlime.vx = 0;
  //  m_bSlime.setX(0);
  //}
  //// m_bSlime-filet
  //if (m_bSlime.getCenter().x+slimeWidth/2 > net.getPosition().x)
  //{
  //  m_bSlime.vx = 0;
  //  m_bSlime.setX(net.getPosition().x-slimeWidth);
  //}

  ////m_rSlime-balle
  //if ( abs2 ( m_rSlime.getCenter()-ball.getCenter() ) <= pow2 ( ballRadius+slimeWidth/2 ) )
  //{
  //  sf::Vector2f newV = symetric(sf::Vector2f(ball.vx, ball.vy), ball.getCenter()-m_rSlime.getCenter());
  //  ball.vx = -newV.x+m_rSlime.vx;
  //  ball.vy = -newV.y+m_rSlime.vy;
  //  sf::Vector2f newPos = (slimeHeight+ballRadius)/(float)sqrt(abs2(ball.getCenter() - m_rSlime.getCenter()))
  //    *(ball.getCenter() - m_rSlime.getCenter()) + rSlime.getCenter()-sf::Vector2f(ballRadius,ballRadius);
  //  ball.setX(newPos.x);
  //  ball.setY(newPos.y);
  //}
  //// m_rSlime-sol
  //if ( m_rSlime.y  > height- slimeHeight )
  //{
  //  m_rSlime.onGround = true;
  //  m_rSlime.vy = 0;
  //  m_rSlime.setY ( height-slimeHeight );
  //}
  //// m_rSlime-mur droit
  //if ( m_rSlime.x > width-slimeWidth)
  //{
  //  m_rSlime.vx = 0;
  //  m_rSlime.setX(width-slimeWidth);
  //}
  //// m_rSlime-filet
  //if (m_rSlime.x < net.getPosition().x+netWidth)
  //{
  //  m_rSlime.vx = 0;
  //  m_rSlime.setX(net.getPosition().x+netWidth);
  //}


  // Balle-salle
  //m_ball.collideRoom(sf::FloatRect(0, 0, WIDTH, HEIGHT));
  //if (ball.x < 0)
  //{
  //  ball.setX(0);
  //  ball.vx = -ball.vx;
  //}
  //else if (ball.x > width-2*ballRadius)
  //{
  //  ball.setX(width-2*ballRadius);
  //  ball.vx = -ball.vx;
  //}
  //if (ball.y > height-2*ballRadius)
  //{
  //  ball.setY(height-2*ballRadius);
  //  ball.vy = -ball.vy;
  //  if (ball.x < net.getPosition().x-netWidth/2) // Qui a perdu ?
  //    m_bSlime.lost = true;
  //  else
  //    m_rSlime.lost = true;
  //  m_clock.restart ();
  //}
  //if (ball.y < 0)
  //{
  //  ball.setY(0);
  //  ball.vy = -ball.vy;
  //}

  //// Balle-filet
  //if (ball.getSprite().getPosition().y+ballRadius >= net.getPosition().y) // Collision horizontale ?
  //{
  //  if (std::abs(ball.getCenter().x - (net.getPosition().x + netWidth/2)) <= netWidth/2)
  //  {
  //    if (ball.vx >= 0)
  //      ball.setX(net.getPosition().x-ballRadius*2);
  //    else
  //      ball.setX(net.getPosition().x+netWidth);
  //    ball.vx = -ball.vx;
  //  }
  //}
  //else if (std::abs(ball.getCenter().x - (net.getPosition().x + netWidth/2)) <= netWidth/2)
  //{
  //  if (ball.getCenter().y >= net.getPosition().y -ballRadius)
  //  {
  //    ball.vy = -std::abs(ball.vy);
  //    ball.setY(net.getPosition().y-2*ballRadius);
  //  }
  //}

  ///*** Gravité ***/
  //ball.vy += gravity*eps;
  //if ( !m_bSlime.onGround )
  //  m_bSlime.vy += gravity*eps;
  //if ( !m_rSlime.onGround )
  //  m_rSlime.vy += gravity*eps;
}

