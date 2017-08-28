#include <catch.hpp>
#include <tuple>
#include <array>
#include "../../game/utils.h"

typedef sf::Vector2f Pt;
typedef sf::Vector2f Sp; // Speed
typedef std::array<float,3> fT; // float triple
typedef std::tuple<Pt, Pt, Sp, Sp, fT> CircPair;

TEST_CASE( "Test de collision entre cercles mouvants", "[collideTwoCircles]")
{
  sf::Vector2f M, Mp;
  sf::Vector2f V, Vp;
  float R, Rp, dt;

  SECTION ("Vitesses parallèles, pas d'intersection")
  {
    CircPair circPairs[] = {
      {Pt(0, 0), Pt(10, 0), Sp(1, 0), Sp(1, 0), {{1, 1, 1}}},
    };

    for (const CircPair &s : circPairs)
    {
      M = std::get<0>(s);
      Mp = std::get<1>(s);
      V = std::get<2>(s);
      Vp = std::get<3>(s);
      R = std::get<4>(s)[0];
      Rp = std::get<4>(s)[1];
      dt = std::get<4>(s)[2];

      INFO("M : " << M.x << ", " << M.y);
      INFO("M' : " << Mp.x << ", " << Mp.y);
      INFO("V : " << V.x << ", " << V.y);
      INFO("V' : " << Vp.x << ", " << Vp.y);
      INFO("R : " << R);
      INFO("R' : " << Rp);
      INFO("dt : " << dt);

      CHECK( collideTwoCircles(M, R, V, Mp, Rp, Vp, dt) == Approx(-1));
    }
  }
}

