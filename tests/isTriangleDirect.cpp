#include <catch.hpp>
#include "../game/utils.h"
#include <limits>

TEST_CASE( "Triangles directs/indirects", "[isTriangleDirect]")
{
  sf::Vector2f A, B, C;
  SECTION( "Triangles ordinaires")
  {
    SECTION( "Triangle rectangle simple" )
    {
      A = sf::Vector2f(0, 0);
      B = sf::Vector2f(1, 0);
      C = sf::Vector2f(1, 1);

      CHECK( isTriangleDirect(A, B, C) );
      CHECK( isTriangleDirect(B, C, A) );
      CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle allongé" )
    {
      A = sf::Vector2f(0, 0);
      B = sf::Vector2f(1, 0);
      C = sf::Vector2f(2, 1);

      CHECK( isTriangleDirect(A, B, C) );
      CHECK( isTriangleDirect(B, C, A) );
      CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle isocèle" )
    {
      A = sf::Vector2f(0, 0);
      B = sf::Vector2f(1, 0);
      C = sf::Vector2f(.5, .5);

      CHECK( isTriangleDirect(A, B, C) );
      CHECK( isTriangleDirect(B, C, A) );
      CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }
  }

  SECTION( "Triangles spéciaux" )
  {
    SECTION( "Triangle aplati" )
    {
      A = sf::Vector2f(0, 0);
      B = sf::Vector2f(1, 0);
      C = sf::Vector2f(2, 0);

      CHECK( isTriangleDirect(A, B, C) );
      CHECK( isTriangleDirect(B, C, A) );
      CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }
  }

  SECTION( "Triangles infinis" )
  {
    float inf = std::numeric_limits<float>::infinity();
    SECTION( "Triangle avec un sommet infini" )
    {
      A = sf::Vector2f(0, 0);
      B = sf::Vector2f(1, 0);
      C = sf::Vector2f(0, +inf);

      CHECK( isTriangleDirect(A, B, C) );
      CHECK( isTriangleDirect(B, C, A) );
      CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec deux sommets infinis" )
    {
      A = sf::Vector2f(0, 0);
      B = sf::Vector2f(1, -inf);
      C = sf::Vector2f(1, +inf);

      CHECK( isTriangleDirect(A, B, C) );
      CHECK( isTriangleDirect(B, C, A) );
      CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec trois sommets infinis" )
    {
      A = sf::Vector2f(-inf, -inf);
      B = sf::Vector2f(+inf, -inf);
      C = sf::Vector2f(+inf, +inf);

      CHECK( isTriangleDirect(A, B, C) );
      CHECK( isTriangleDirect(B, C, A) );
      CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }
  }

  SECTION( "Triangles avec bords indéfinis" )
  {
    float nan = std::numeric_limits<float>::quiet_NaN();
    SECTION( "Triangle avec un sommet indéfini" )
    {
      A = sf::Vector2f(0, nan);
      B = sf::Vector2f(1, 0);
      C = sf::Vector2f(1, 1);

      CHECK_FALSE ( isTriangleDirect(A, B, C) );
      CHECK_FALSE ( isTriangleDirect(B, C, A) );
      CHECK_FALSE ( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec deux sommets indéfinis" )
    {
      A = sf::Vector2f(0, nan);
      B = sf::Vector2f(1, nan);
      C = sf::Vector2f(1, 1);

      CHECK_FALSE ( isTriangleDirect(A, B, C) );
      CHECK_FALSE ( isTriangleDirect(B, C, A) );
      CHECK_FALSE ( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec trois sommets indéfinis" )
    {
      A = sf::Vector2f(0, nan);
      B = sf::Vector2f(1, nan);
      C = sf::Vector2f(1, nan);

      CHECK_FALSE ( isTriangleDirect(A, B, C) );
      CHECK_FALSE ( isTriangleDirect(B, C, A) );
      CHECK_FALSE ( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }
  }
}

