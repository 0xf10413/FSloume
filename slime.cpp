#include "slime.h"

Slime::Slime ( bool isLeft ) : left ( isLeft ), onGround ( true )
{
  x = y = vx = vy = 0;
	victories = lost = false;
  image.Create ( slimeWidth, slimeHeight, isLeft ? sf::Color::Blue : sf::Color::Red );
  // Dessin de la forme hémi-circulaire caractéristique
  // Calcul : tout pixel distant du point au milieu (horizontalement)
  // en bas (verticalement) de plus de width/2 est transparent.
  for ( int i = 0; i < slimeWidth; i++ )
    for ( int j=0; j<slimeHeight; j++ )
      if ( ( i - slimeWidth/2 ) * ( i - slimeWidth/2 ) +
           ( j - slimeHeight ) * ( j-slimeHeight ) >=
           ( slimeWidth*slimeWidth/4 ) )
        image.SetPixel ( i, j, sf::Color ( 0, 0, 0, 0 ) );

  if ( isLeft )
    {
      // Dessin du globe oculaire (coordonnées locales du centre : (3*w/4, h/2))
      for ( int i = 0; i < slimeWidth; i++ )
        for ( int j=0; j < slimeHeight; j++ )
          if ( ( i - 3*slimeWidth/4 ) * ( i - 3*slimeWidth/4 ) +
               ( j - slimeHeight/2 ) * ( j-slimeHeight/2 ) <=
               eyeRadiusSquared )
            image.SetPixel ( i, j, sf::Color::White );
    }
  else
    // Dessin du globe oculaire (coordonnées locales du centre : (w/4, h/2))
    for ( int i = 0; i < slimeWidth; i++ )
      for ( int j=0; j < slimeHeight; j++ )
        if ( ( i - slimeWidth/4 ) * ( i - slimeWidth/4 ) +
             ( j - slimeHeight/2 ) * ( j-slimeHeight/2 ) <=
             eyeRadiusSquared )
          image.SetPixel ( i, j, sf::Color::White );

  eyeImage.Create ( pupilRadius*2,pupilRadius*2, isLeft ? sf::Color::Cyan : sf::Color::Magenta );
  makeADisk ( eyeImage );
  sprite.SetImage ( image );
  eyeSprite.SetImage ( eyeImage );
}

// Renvoie le point d'intérêt du Slime
sf::Vector2f Slime::getCenter()
{
  sf::Vector2f center;
  center.x = sprite.GetPosition().x + slimeWidth/2;
  center.y = sprite.GetPosition().y + slimeHeight;
  return center;
}

void Slime::updateEye ( const sf::Vector2f& b )
{
  sf::Vector2f newPos;
  // Globe oculaire, de coordonnées locales (3*w/4, h/2)
  if ( left )
      newPos = getCenter() +sf::Vector2f ( slimeWidth/4,-slimeHeight/2 )
               + ( sqrt ( eyeRadiusSquared )-pupilRadius ) * ( b- ( getCenter() +sf::Vector2f ( slimeWidth/4,-slimeHeight/2 ) ) )
               /sqrt ( abs2 ( b- ( getCenter() +sf::Vector2f ( slimeWidth/4,-slimeHeight/2 ) ) ) )
               - sf::Vector2f ( pupilRadius,pupilRadius );
  else
    newPos = getCenter()+sf::Vector2f ( -slimeWidth/4,-slimeHeight/2 )
             + ( sqrt ( eyeRadiusSquared )-pupilRadius ) * ( b- ( getCenter() +sf::Vector2f ( slimeWidth/4,-slimeHeight/2 ) ) )
             /sqrt ( abs2 ( b- ( getCenter() +sf::Vector2f ( slimeWidth/4,-slimeHeight/2 ) ) ) )
             - sf::Vector2f ( pupilRadius,pupilRadius );
  eyeSprite.SetPosition ( newPos );
}


