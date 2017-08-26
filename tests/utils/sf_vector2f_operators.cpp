#include <catch.hpp>
#include <SFML/System/Vector2.hpp>
#include "../../game/utils.h"
#include <limits>

TEST_CASE( "Opérations supplémentaires sur sf::Vector2f", "[operators_sf_vector2f]")
{
  sf::Vector2f u, v;
  float r;
  sf::Vector2f result;

  SECTION( "Opérateur +" )
  {
    SECTION( "0 est neutre")
    {
      u = sf::Vector2f (1.f, 2.f);
      v = sf::Vector2f (0.f, 0.f);
      result = u + v;

      CHECK( result.x == Approx(1.f));
      CHECK( result.y == Approx(2.f));
    }

    SECTION( "Test de somme" )
    {
      u = sf::Vector2f(10.f, 11.f);
      v = sf::Vector2f(5.f, 8.f);
      result = u + v;

      CHECK( result.x == Approx(15.f));
      CHECK( result.y == Approx(19.f));
    }

    SECTION( "Comportement avec les NaNs")
    {
      float nan = std::numeric_limits<float>::quiet_NaN();
      u = sf::Vector2f (nan, 0.);
      v = sf::Vector2f (0., nan);
      result = u + v;

      CHECK_FALSE( result.x == result.x);
      CHECK_FALSE( result.y == result.y);
    }

  }

  SECTION( "Operateur -" )
  {
    SECTION( "0 est neutre")
    {
      u = sf::Vector2f (1.f, 2.f);
      v = sf::Vector2f (0.f, 0.f);
      result = u - v;

      CHECK( result.x == Approx(1.f));
      CHECK( result.y == Approx(2.f));
    }

    SECTION( "u - u = 0")
    {
      u = sf::Vector2f (1.f, 2.f);
      v = sf::Vector2f (1.f, 2.f);
      result = u - v;

      CHECK( result.x == Approx(0.f));
      CHECK( result.y == Approx(0.f));
    }

    SECTION( "Comportement avec les NaNs")
    {
      float nan = std::numeric_limits<float>::quiet_NaN();
      u = sf::Vector2f (nan, 0.);
      v = sf::Vector2f (0., nan);
      result = u - v;

      CHECK_FALSE( result.x == result.x);
      CHECK_FALSE( result.y == result.y);
    }
  }

  SECTION( "Opérateur *")
  {
    SECTION( "1 est neutre")
    {
      u = sf::Vector2f (1.f, 2.f);
      r = 1.f;
      result = r*u;

      CHECK( result.x == Approx(1.f));
      CHECK( result.y == Approx(2.f));
    }

    SECTION( "0 annule les coordonées")
    {
      u = sf::Vector2f (1.f, 2.f);
      r = 0.f;
      result = r*u;

      CHECK( result.x == Approx(0.f));
      CHECK( result.y == Approx(0.f));
    }

    SECTION( "Test de produit" )
    {
      u = sf::Vector2f(10.f, 11.f);
      r = 2.f;
      result = r*u;

      CHECK( result.x == Approx(20.f));
      CHECK( result.y == Approx(22.f));
    }

    SECTION( "Comportement avec les NaNs")
    {
      float nan = std::numeric_limits<float>::quiet_NaN();
      u = sf::Vector2f (nan, 0.);
      r = 1;
      result = r*u;

      CHECK_FALSE( result.x == result.x);
      CHECK( result.y == result.y);
    }
  }

  SECTION( "Opérateur /")
  {
    SECTION( "1 est neutre")
    {
      u = sf::Vector2f (1.f, 2.f);
      r = 1.f;
      result = u/r;

      CHECK( result.x == Approx(1.f));
      CHECK( result.y == Approx(2.f));
    }

    SECTION( "Test de quotient" )
    {
      u = sf::Vector2f(10.f, 12.f);
      r = 2.f;
      result = u/r;

      CHECK( result.x == Approx(5.f));
      CHECK( result.y == Approx(6.f));
    }

    SECTION( "Comportement avec les NaNs")
    {
      float nan = std::numeric_limits<float>::quiet_NaN();
      u = sf::Vector2f (nan, 0.);
      r = 1;
      result = r*u;

      CHECK_FALSE( result.x == result.x);
      CHECK( result.y == result.y);
    }
  }

  SECTION( "Déterminant det" )
  {
    SECTION( "Colinéarité au vecteur nul")
    {
      u = sf::Vector2f(0.f, 0.f);
      v = sf::Vector2f (12.f, 15.f);

      CHECK (det(u,v) == Approx(0.f));
    }

    SECTION( "Colinéarité de deux vecteurs")
    {
      u = sf::Vector2f(1.f, 2.f);
      v = sf::Vector2f (4.f, 8.f);

      CHECK (det(u,v) == Approx(0.f));
    }

    SECTION( "Non-colinéarité de deux vecteurs")
    {
      u = sf::Vector2f(1.f, 2.f);
      v = sf::Vector2f (-4.f, 8.f);

      CHECK_FALSE (det(u,v) == Approx(0.f));
    }
  }

  SECTION( "Produit scalaire dotProduct" )
  {
    SECTION( "Orthogonalité au vecteur nul")
    {
      u = sf::Vector2f(0.f, 0.f);
      v = sf::Vector2f (12.f, 15.f);

      CHECK (dotProduct(u,v) == Approx(0.f));
    }

    SECTION( "Orthogonalité de deux vecteurs")
    {
      u = sf::Vector2f(1.f, 2.f);
      v = sf::Vector2f (-2.f, 1.f);

      CHECK (dotProduct(u,v) == Approx(0.f));
    }

    SECTION( "Non-orthogonalité de deux vecteurs")
    {
      u = sf::Vector2f(1.f, 2.f);
      v = sf::Vector2f (4.f, 8.f);

      CHECK_FALSE (dotProduct(u,v) == Approx(0.f));
    }
  }

  SECTION( "Norme 2 norm2" )
  {
    SECTION( "Norme du vecteur nul")
    {
      u = sf::Vector2f(0.f, 0.f);

      CHECK (norm2(u) == Approx(0.f));
    }

    SECTION( "Norme 2 d'un vecteur")
    {
      u = sf::Vector2f(3.f, 4.f);

      CHECK (norm2(u) == Approx(5.f));
    }

    SECTION( "Norme 2 d'un autre vecteur")
    {
      u = sf::Vector2f (-3.f, 4.f);

      CHECK (norm2(u) == Approx(5.f));
    }
  }

  SECTION( "Norme 2 au carré abs2")
  {
    SECTION( "Norme2 carré du vecteur nul")
    {
      u = sf::Vector2f(0.f, 0.f);

      CHECK (abs2(u) == Approx(0.f));
    }

    SECTION("Norme2 carré d'un autre vecteur")
    {
      u = sf::Vector2f(3.f, 4.f);

      CHECK( abs2(u) == Approx(25.f));
    }

    SECTION("Lien avec norm2")
    {
      u = sf::Vector2f(3.f, 6.f);

      CHECK (abs2(u) == Approx(pow2(norm2(u))));
    }

    SECTION("Lien avec dotProduct")
    {
      u = sf::Vector2f(3.f, 6.f);

      CHECK (abs2(u) == Approx(dotProduct(u,u)));
    }

  }
}
