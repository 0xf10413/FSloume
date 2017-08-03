#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
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


int main ()
{
	sf::RenderWindow app ( sf::VideoMode ( width, height ), "sfml" );
	app.setFramerateLimit ( 60 );
	sf::Event event;
	sf::Clock clock;
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf");
    class { public: bool IsKeyDown (sf::Keyboard::Key ) { return false; } } input;

	Slime bSlime ( true );
	bSlime.setX ( width/4-slimeWidth/2 );
	bSlime.setY ( height-slimeHeight );

	Slime rSlime ( false );
	rSlime.setX ( 3*width/4-slimeWidth/2 );
	rSlime.setY ( height-slimeHeight );

	sf::Image netImage;
    netImage.create ( netWidth, netHeight, sf::Color::Yellow );
    sf::Texture netTexture;
    netTexture.loadFromImage(netImage);
	sf::Sprite net ( netTexture );
	net.setPosition ( ( width-netWidth ) /2,  height-netHeight );

	Ball ball;
	ball.setX ( bSlime.x+slimeWidth/2-ballRadius );
	ball.setY ( height/2 );

	bool reinit = false; // Faut-il tout replacer ?
	// Boucle principale
	while ( app.isOpen() )
	{
		float eps = clock.getElapsedTime().asSeconds();

		// Réinitialisation de la partie
		if (reinit)
		{
			bSlime.setX ( width/4-slimeWidth/2 );
			bSlime.setY ( height-slimeHeight );

			rSlime.setX ( 3*width/4-slimeWidth/2 );
			rSlime.setY ( height-slimeHeight );

			if (rSlime.lost)
				ball.setX ( bSlime.x+slimeWidth/2-ballRadius );
			else
				ball.setX ( rSlime.x+slimeWidth/2-ballRadius );

			ball.setY ( height/2 );

			ball.vx = ball.vy = 0;
			bSlime.vx = bSlime.vy = 0;
			rSlime.vx = rSlime.vy = 0;

			reinit = bSlime.lost = rSlime.lost = false;
		}

		if (bSlime.lost || rSlime.lost) // Quelqu'un a perdu ?
		{
			sf::Text text ("Perdu !", font);
			if (bSlime.lost)
				text.setFillColor (sf::Color::Blue);
			else
				text.setFillColor (sf::Color::Red);

			app.draw (text);
			app.display ();
			if (clock.getElapsedTime().asSeconds() < 1.)
			  continue;

			reinit = true;
			continue;
		}

		while ( app.pollEvent ( event ) )
		{
			if ( event.type == sf::Event::Closed )
				app.close();
			if ( event.type == sf::Event::KeyPressed )
			{
				if ( event.key.code == sf::Keyboard::Escape )
					app.close();
				if ( event.key.code == sf::Keyboard::Up )
				{
					if ( bSlime.onGround )
					{
						bSlime.vy += -20;
						bSlime.onGround = false;
					}
				}
				if (event.key.code == sf::Keyboard::Space)
				{
					if (rSlime.onGround)
					{
						rSlime.vy += -20;
						rSlime.onGround = false;
					}
				}


			}
		}

		if ( input.IsKeyDown ( sf::Keyboard::Left ) && !input.IsKeyDown ( sf::Keyboard::Right ) )
			bSlime.vx = -slimeHorizontalSpeed;
		else if ( input.IsKeyDown ( sf::Keyboard::Right ) && !input.IsKeyDown(sf::Keyboard::Left))
			bSlime.vx = slimeHorizontalSpeed;
		else
			bSlime.vx = 0;

		if ( input.IsKeyDown ( sf::Keyboard::Q ) && !input.IsKeyDown ( sf::Keyboard::D ) )
			rSlime.vx = -slimeHorizontalSpeed;
		else if ( input.IsKeyDown ( sf::Keyboard::D ) && !input.IsKeyDown(sf::Keyboard::Q) )
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
		// TODO: limiter la vitesse
		ball.setY ( ball.y + ball.vy*eps );
		ball.setX ( ball.x + ball.vx*eps );


		/*** Collisions ***/
		// bSlime-balle
		if ( abs2 ( bSlime.getCenter()-ball.getCenter() ) <= pow2 ( ballRadius+slimeWidth/2 ) )
		{
			sf::Vector2f newV = symetric(sf::Vector2f(ball.vx, ball.vy), ball.getCenter()-bSlime.getCenter());
			ball.vx = -newV.x+bSlime.vx;
			ball.vy = -newV.y+bSlime.vy;
			sf::Vector2f newPos = (slimeHeight+ballRadius)/(float)sqrt(abs2(ball.getCenter() - bSlime.getCenter()))
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
		if (bSlime.getCenter().x+slimeWidth/2 > net.getPosition().x)
		{
			bSlime.vx = 0;
			bSlime.setX(net.getPosition().x-slimeWidth);
		}

		//rSlime-balle
		if ( abs2 ( rSlime.getCenter()-ball.getCenter() ) <= pow2 ( ballRadius+slimeWidth/2 ) )
		{
			sf::Vector2f newV = symetric(sf::Vector2f(ball.vx, ball.vy), ball.getCenter()-rSlime.getCenter());
			ball.vx = -newV.x+rSlime.vx;
			ball.vy = -newV.y+rSlime.vy;
			sf::Vector2f newPos = (slimeHeight+ballRadius)/(float)sqrt(abs2(ball.getCenter() - rSlime.getCenter()))
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
		if (rSlime.x < net.getPosition().x+netWidth)
		{
			rSlime.vx = 0;
			rSlime.setX(net.getPosition().x+netWidth);
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
			if (ball.x < net.getPosition().x-netWidth/2) // Qui a perdu ?
				bSlime.lost = true;
			else
				rSlime.lost = true;
			clock.restart ();
		}
		if (ball.y < 0)
		{
			ball.setY(0);
			ball.vy = -ball.vy;
		}

		// Balle-filet
		if (ball.getSprite().getPosition().y+ballRadius >= net.getPosition().y) // Collision horizontale ?
		{
			if (std::abs(ball.getCenter().x -(float)(net.getPosition().x + netWidth/2)) <= netWidth/2)
			{
				if (ball.vx >= 0)
					ball.setX(net.getPosition().x-ballRadius*2);
				else
					ball.setX(net.getPosition().x+netWidth);
				ball.vx = -ball.vx;
			}
		}
		else if (std::abs(ball.getCenter().x - (net.getPosition().x + netWidth/2)) <= netWidth/2)
		{
			if (ball.getCenter().y >= net.getPosition().y -ballRadius)
			{
				ball.vy = -std::abs(ball.vy);
				ball.setY(net.getPosition().y-2*ballRadius);
			}
		}

		// Affichage
		bSlime.updateEye(ball.getCenter());
		rSlime.updateEye(ball.getCenter());
		app.clear();
		app.draw ( rSlime.getSprite() );
		app.draw(rSlime.getEye());
		app.draw ( bSlime.getSprite() );
		app.draw ( bSlime.getEye() );
		app.draw ( net );
		app.draw ( ball.getSprite() );
		app.display();
	}
	return EXIT_SUCCESS;
}
