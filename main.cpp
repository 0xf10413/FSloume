#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "main.h"
#include "slime.h"
#include "ball.h"

using std::cout;
using std::endl;
using std::cerr;

/*** BUG ***/
/* 
 * Collision par dessous : un slime entre en collision
 * de façon circulaire avec la balle par en-dessous.
 * Balle fantôme : la balle peut passer à travers le filet 
 * si elle va assez vite. Solutions : examiner x+vx*dt, majorer vx.
 * */
/*** TODO ***/
/*
 * Une IA
 * Un menu d'accueil
 * Une gestion des scores
 * Des ralentisseurs/objets ?
 */


int main ( int argc, char **argv )
{
  sf::RenderWindow app ( sf::VideoMode ( width, height ), "sfml" );
  app.SetFramerateLimit ( 60 );
  sf::Event event;
  sf::Clock clock;

  Slime bSlime ( true );
  bSlime.setX ( width/4-slimeWidth/2 );
  bSlime.setY ( height-slimeHeight );

  Slime rSlime ( false );
  rSlime.setX ( 3*width/4-slimeWidth/2 );
  rSlime.setY ( height-slimeHeight );

  sf::Image netImage ( netWidth, netHeight, sf::Color::Yellow );
  sf::Sprite net ( netImage );
  net.SetX ( ( width-netWidth ) /2 );
  net.SetY ( height-netHeight );

  Ball ball;
  ball.setX ( bSlime.x+slimeWidth/2-ballRadius );
  ball.setY ( height/2 );
  
  const sf::Input &input = app.GetInput();
  // Boucle principale
  while ( app.IsOpened() )
    {
      float eps = app.GetFrameTime();

      // Boucle d'évènements
      while ( app.GetEvent ( event ) )
        {
          if ( event.Type == sf::Event::Closed )
            app.Close();
          if ( event.Type == sf::Event::KeyPressed )
            {
              if ( event.Key.Code == sf::Key::Escape )
                app.Close();
              if ( event.Key.Code == sf::Key::Up )
                {
                  if ( bSlime.onGround )
                    {
                      bSlime.vy += -200;
                      bSlime.onGround = false;
                    }
                }
                if (event.Key.Code == sf::Key::Space)
		{
			if (rSlime.onGround)
			{
			rSlime.vy += -200;
			rSlime.onGround = false;
			}
		}
                

            }
        }

      if ( input.IsKeyDown ( sf::Key::Left ) && !input.IsKeyDown ( sf::Key::Right ) )
        bSlime.vx = -slimeHorizontalSpeed;
      else if ( input.IsKeyDown ( sf::Key::Right ) && !input.IsKeyDown(sf::Key::Left))
        bSlime.vx = slimeHorizontalSpeed;
      else
        bSlime.vx = 0;
      
       if ( input.IsKeyDown ( sf::Key::Q ) && !input.IsKeyDown ( sf::Key::D ) )
        rSlime.vx = -slimeHorizontalSpeed;
      else if ( input.IsKeyDown ( sf::Key::D ) && !input.IsKeyDown(sf::Key::Q) )
        rSlime.vx = slimeHorizontalSpeed;
      else
        rSlime.vx = 0;


      /********* Mouvement************/
      /*** Gravité ***/
      ball.vy += gravity*eps;
      if ( !bSlime.onGround )
        bSlime.vy += gravity*eps;
      if ( !rSlime.onGround )
        rSlime.vy += gravity*eps;

      /*** Déplacement des objets ***/
      // bSlime
      bSlime.setX ( bSlime.x + bSlime.vx*eps );
      bSlime.setY ( bSlime.y + bSlime.vy*eps );

      // rSlime
      rSlime.setX ( rSlime.x + rSlime.vx*eps );
      rSlime.setY ( rSlime.y + rSlime.vy*eps );
      // Le déplacement des pupilles se fait à la fin


      // Balle
      ball.setY ( ball.y + ball.vy*eps );
      ball.setX ( ball.x + ball.vx*eps );


      /*** Collisions ***/
      // bSlime-balle
      if ( abs2 ( bSlime.getCenter()-ball.getCenter() ) <= pow2 ( ballRadius+slimeWidth/2 ) )
        {
	  sf::Vector2f newV = symetric(sf::Vector2f(ball.vx, ball.vy), ball.getCenter()-bSlime.getCenter());
	  ball.vx = -newV.x+bSlime.vx;
	  ball.vy = -newV.y+bSlime.vy;
	  sf::Vector2f newPos = (slimeHeight+ballRadius)/sqrt(abs2(ball.getCenter() - bSlime.getCenter()))
	  *(ball.getCenter() - bSlime.getCenter()) + bSlime.getCenter()-sf::Vector2f(ballRadius,ballRadius);
	  ball.setX(newPos.x);
	  ball.setY(newPos.y);
        }
        
      // bSlime-sol
      if ( bSlime.y  > height- slimeHeight )
        {
          bSlime.onGround = true;
          bSlime.vy = 0;
          bSlime.setY ( height-slimeHeight );
        }
        // bSlime-mur gauche
        if ( bSlime.x < 0)
	{
		bSlime.vx = 0;
		bSlime.setX(0);
	}
	// bSlime-filet
	if (bSlime.getCenter().x+slimeWidth/2 > net.GetPosition().x)
	{
		bSlime.vx = 0;
		bSlime.setX(net.GetPosition().x-slimeWidth);
	}
	
	//rSlime-balle
         if ( abs2 ( rSlime.getCenter()-ball.getCenter() ) <= pow2 ( ballRadius+slimeWidth/2 ) )
        {  
	  sf::Vector2f newV = symetric(sf::Vector2f(ball.vx, ball.vy), ball.getCenter()-rSlime.getCenter());
	  ball.vx = -newV.x+rSlime.vx;
	  ball.vy = -newV.y+rSlime.vy;
	  sf::Vector2f newPos = (slimeHeight+ballRadius)/sqrt(abs2(ball.getCenter() - rSlime.getCenter()))
	  *(ball.getCenter() - rSlime.getCenter()) + rSlime.getCenter()-sf::Vector2f(ballRadius,ballRadius);
	  ball.setX(newPos.x);
	  ball.setY(newPos.y);
        }
        // rSlime-sol
      if ( rSlime.y  > height- slimeHeight )
        {
          rSlime.onGround = true;
          rSlime.vy = 0;
          rSlime.setY ( height-slimeHeight );
        }
        // rSlime-mur droit
        if ( rSlime.x > width-slimeWidth)
	{
		rSlime.vx = 0;
		rSlime.setX(width-slimeWidth);
	}
	// rSlime-filet
	if (rSlime.x < net.GetPosition().x+netWidth)
	{
		rSlime.vx = 0;
		rSlime.setX(net.GetPosition().x+netWidth);
	}
	
        
        // Balle-salle
        if (ball.x < 0)
	{
		ball.setX(0);
		ball.vx = -ball.vx;
	}
	else if (ball.x > width-2*ballRadius)
	{
		ball.setX(width-2*ballRadius);
		ball.vx = -ball.vx;
	}
	if (ball.y > height-2*ballRadius)
	{
		ball.setY(height-2*ballRadius);
		ball.vy = -ball.vy;
	}
	if (ball.y < 0)
	{
		ball.setY(0);
		ball.vy = -ball.vy;
	}
	
	// Balle-filet
	if (ball.getSprite().GetPosition().y+ballRadius >= net.GetPosition().y) // Collision horizontale ?
	{
		if (abs(ball.getCenter().x - (net.GetPosition().x + netWidth/2)) <= netWidth/2)
		{
			if (ball.vx >= 0)
				ball.setX(net.GetPosition().x-ballRadius*2);
			else
				ball.setX(net.GetPosition().x+netWidth);
			ball.vx = -ball.vx;
		}
	}
	else if (abs(ball.getCenter().x - (net.GetPosition().x + netWidth/2)) <= netWidth/2)
	{
		if (ball.getCenter().y >= net.GetPosition().y -ballRadius)
		{
			ball.vy = -abs(ball.vy);
 			ball.setY(net.GetPosition().y-2*ballRadius);
		}
	}

      // Affichage
            bSlime.updateEye(ball.getCenter());
	          rSlime.updateEye(ball.getCenter());
      app.Clear();
      app.Draw ( rSlime.getSprite() );
      app.Draw(rSlime.getEye());
      app.Draw ( bSlime.getSprite() );
      app.Draw ( bSlime.getEye() );
      app.Draw ( net );
      app.Draw ( ball.getSprite() );
      app.Display();
    }
  return EXIT_SUCCESS;
}
