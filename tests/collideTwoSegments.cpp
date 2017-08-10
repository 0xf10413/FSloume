#include <catch.hpp>
#include "../game/utils.h"
#include "../game/config.h"

typedef sf::Vector2f Pt;
typedef sf::Vector2f SegPair[4];

TEST_CASE( "Test de collision entre segments", "[collideTwoSegments]")
{
  sf::Vector2f A, B, C, D;
  SECTION ("Segments parallèles")
  {
    SegPair segPairs[] = {
      {Pt(0, 0), Pt(1, 0), Pt(0, 1), Pt(1,1)},
      {Pt(0, 0), Pt(100, 0), Pt(.5f, 1), Pt(.6f,1)},
      {Pt(0, 0), Pt(1, 0), Pt(5, 1), Pt(600,1)},
      {Pt(0, 0), Pt(0, 1), Pt(-.1f, 1), Pt(-.1f,2)},
    };

    for (const SegPair &s : segPairs)
    {
      A = s[0];
      B = s[1];
      C = s[2];
      D = s[3];
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);
      INFO("D : " << D.x << ", " << D.y);

      CHECK_FALSE( collideTwoSegments(A, B, C, D));
    }
  }

  SECTION ("Segments avec un point commun")
  {
    SegPair segPairs[] = {
      {Pt(0, 0), Pt(1, 0), Pt(0, 0), Pt(1,1)},
      {Pt(0, 0), Pt(100, 0), Pt(100, 0), Pt(.6f,1)},
    };

    for (const SegPair &s : segPairs)
    {
      A = s[0];
      B = s[1];
      C = s[2];
      D = s[3];
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);
      INFO("D : " << D.x << ", " << D.y);

      CHECK( collideTwoSegments(A, B, C, D));
    }
  }

  SECTION ("Segments orthogonaux")
  {
    SegPair segPairs[] = {
      {Pt(0, 0), Pt(1, 0), Pt(.5, -1), Pt(.5,1)},
      {Pt(0, 0), Pt(1, 1), Pt(0, 1), Pt(1,0)},
    };

    for (const SegPair &s : segPairs)
    {
      A = s[0];
      B = s[1];
      C = s[2];
      D = s[3];
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);
      INFO("D : " << D.x << ", " << D.y);

      CHECK( collideTwoSegments(A, B, C, D));
    }
  }

  SECTION ("Segments non-sécants quelconques")
  {
    SegPair segPairs[] = {
      {Pt(0, 0), Pt(1, 4), Pt(5, -1), Pt(-1,10)},
      {Pt(-10, 1), Pt(5, -10), Pt(-5, 1), Pt(3,100)},
    };

    for (const SegPair &s : segPairs)
    {
      A = s[0];
      B = s[1];
      C = s[2];
      D = s[3];
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);
      INFO("D : " << D.x << ", " << D.y);

      CHECK_FALSE( collideTwoSegments(A, B, C, D));
    }
  }
}

