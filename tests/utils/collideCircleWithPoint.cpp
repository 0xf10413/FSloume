#include <catch2/catch.hpp>
#include <tuple>
#include <array>
#include <SFML/System/Vector3.hpp>
#include "../../game/tools/utils.h"

typedef sf::Vector2f Pt; // Point
typedef sf::Vector2f Sp; // Speed
/* M, R, V, M', dt, collision expected */
typedef std::tuple<Pt, float, Sp, Pt, float, bool> TestData;

TEST_CASE( "Test de collision entre cercle mouvant et point fixe", "[collideCircleWithPoint]")
{
  const float inf = std::numeric_limits<float>::infinity();
  Pt M, Mp;
  float R;
  Sp V;
  float dt;
  bool expect;

#define AFFECT_SYMBOLS_FROM(s) \
  M = std::get<0>(s); \
  R = std::get<1>(s); \
  V = std::get<2>(s); \
  Mp = std::get<3>(s); \
  dt = std::get<4>(s); \
  expect = std::get<5>(s);


  SECTION ("Pas d'intersection")
  {
    TestData testData[] = {
      {Pt(0,0), 1, Sp(1,0), Pt(2,-2), 100, false}, // Ordinaire
      {Pt(1,0), 0, Sp(1,0), Pt(0,-1), 100, false}, // Rayon nul
      {Pt(0,0), 1, Sp(1,0), Pt(11,0), 9, false}, // Trop tard
    };

    for (const TestData &d : testData)
    {
      AFFECT_SYMBOLS_FROM(d);
      INFO("M : " << M.x << ", " << M.y);
      INFO("R : " << R);
      INFO("V : " << V.x << ", " << V.y);
      INFO("M' : " << Mp.x << ", " << Mp.y);
      INFO("dt : " << dt);
      INFO("expect : " << expect);

      CHECK( collideCircleWithPoint(M, R, V, Mp, dt) == expect );
    }
  }

  SECTION ("Intersection")
  {
    TestData testData[] = {
      {Pt(0,0), 1, Sp(1,1), Pt(5,5), 100, true}, // Ordinaire
      {Pt(0,0), 0, Sp(1,0), Pt(10,0), 10, true}, // Rayon nul
      {Pt(10,10), 0, Sp(-1,-1), Pt(5,5), 10, true}, // Diagonale (regression test)
    };

    for (const TestData &d : testData)
    {
      AFFECT_SYMBOLS_FROM(d);
      INFO("M : " << M.x << ", " << M.y);
      INFO("R : " << R);
      INFO("V : " << V.x << ", " << V.y);
      INFO("M' : " << Mp.x << ", " << Mp.y);
      INFO("dt : " << dt);
      INFO("expect : " << expect);

      CHECK( collideCircleWithPoint(M, R, V, Mp, dt) == expect );
    }
  }
}
