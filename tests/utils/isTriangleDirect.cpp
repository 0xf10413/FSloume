#include <catch2/catch.hpp>
#include "../../game/tools/utils.h"
#include <limits>

typedef sf::Vector2f Pt;
typedef sf::Vector2f Triangle[3];

TEST_CASE( "Triangles directs/indirects", "[isTriangleDirect]")
{
  Pt A, B, C;
  SECTION( "Triangles ordinaires")
  {
    SECTION( "Triangles rectangles simples" )
    {
      Triangle triangles[] = {
        {{0,0}, {1,0}, {1,1}},
        {{0,0}, {1,1}, {0,1}},
        {{0,-1}, {0,0}, {-1,0}},
      };

      for (const Triangle &t : triangles)
      {
        A = t[0];
        B = t[1];
        C = t[2];
        INFO("A : " << A.x << ", " << A.y);
        INFO("B : " << B.x << ", " << B.y);
        INFO("C : " << C.x << ", " << C.y);

        CHECK( isTriangleDirect(A, B, C) );
        CHECK( isTriangleDirect(B, C, A) );
        CHECK( isTriangleDirect(C, A, B) );

        CHECK_FALSE( isTriangleDirect(A, C, B) );
        CHECK_FALSE( isTriangleDirect(C, B, A) );
        CHECK_FALSE( isTriangleDirect(B, A, C) );
      }
    }

    SECTION( "Triangles allongés" )
    {
      Triangle triangles[] = {
        {{0,0}, {1,0}, {2,1}},
        {{0,0}, {5,-1}, {10,5}},
        {{-1,-1}, {2,-3}, {0,0}},
      };

      for (const Triangle &t : triangles)
      {
        A = t[0];
        B = t[1];
        C = t[2];
        INFO("A : " << A.x << ", " << A.y);
        INFO("B : " << B.x << ", " << B.y);
        INFO("C : " << C.x << ", " << C.y);

        CHECK( isTriangleDirect(A, B, C) );
        CHECK( isTriangleDirect(B, C, A) );
        CHECK( isTriangleDirect(C, A, B) );

        CHECK_FALSE( isTriangleDirect(A, C, B) );
        CHECK_FALSE( isTriangleDirect(C, B, A) );
        CHECK_FALSE( isTriangleDirect(B, A, C) );
      }
    }

    SECTION( "Triangles isocèles" )
    {
      Triangle triangles[] = {
        {{0,0}, {1,0}, {.5,.5}},
        {{-10,0}, {10,0}, {0,5}},
        {{-1,-5}, {1,-4}, {-1,-3}},
      };

      for (const Triangle &t : triangles)
      {
        A = t[0];
        B = t[1];
        C = t[2];
        INFO("A : " << A.x << ", " << A.y);
        INFO("B : " << B.x << ", " << B.y);
        INFO("C : " << C.x << ", " << C.y);

        CHECK( isTriangleDirect(A, B, C) );
        CHECK( isTriangleDirect(B, C, A) );
        CHECK( isTriangleDirect(C, A, B) );

        CHECK_FALSE( isTriangleDirect(A, C, B) );
        CHECK_FALSE( isTriangleDirect(C, B, A) );
        CHECK_FALSE( isTriangleDirect(B, A, C) );
      }
    }
  }

  SECTION( "Triangles spéciaux" )
  {
    SECTION( "Triangles aplatis" )
    {
      Triangle triangles[] = {
        {{0,0}, {1,0}, {2,0}},
        {{0,0}, {1,1}, {2,2}},
        {{-1,-6}, {-1,-4}, {-1,-1}},
      };

      for (const Triangle &t : triangles)
      {
        A = t[0];
        B = t[1];
        C = t[2];
        INFO("A : " << A.x << ", " << A.y);
        INFO("B : " << B.x << ", " << B.y);
        INFO("C : " << C.x << ", " << C.y);

        CHECK( isTriangleDirect(A, B, C) );
        CHECK( isTriangleDirect(B, C, A) );
        CHECK( isTriangleDirect(C, A, B) );

        CHECK_FALSE( isTriangleDirect(A, C, B) );
        CHECK_FALSE( isTriangleDirect(C, B, A) );
        CHECK_FALSE( isTriangleDirect(B, A, C) );
      }
    }
  }

  SECTION( "Triangles infinis" )
  {
    float inf = std::numeric_limits<float>::infinity();
    SECTION( "Triangle avec un sommet infini" )
    {
      A = Pt(0, 0);
      B = Pt(1, 0);
      C = Pt(0, +inf);
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);

      //CHECK( isTriangleDirect(A, B, C) );
      //CHECK( isTriangleDirect(B, C, A) );
      //CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec deux sommets infinis" )
    {
      A = Pt(0, 0);
      B = Pt(1, -inf);
      C = Pt(1, +inf);
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);

      //CHECK( isTriangleDirect(A, B, C) );
      //CHECK( isTriangleDirect(B, C, A) );
      //CHECK( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec trois sommets infinis" )
    {
      A = Pt(-inf, -inf);
      B = Pt(+inf, -inf);
      C = Pt(+inf, +inf);
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);

      //CHECK( isTriangleDirect(A, B, C) );
      //CHECK( isTriangleDirect(B, C, A) );
      //CHECK( isTriangleDirect(C, A, B) );

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
      A = Pt(0, nan);
      B = Pt(1, 0);
      C = Pt(1, 1);
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);

      CHECK_FALSE ( isTriangleDirect(A, B, C) );
      CHECK_FALSE ( isTriangleDirect(B, C, A) );
      CHECK_FALSE ( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec deux sommets indéfinis" )
    {
      A = Pt(0, nan);
      B = Pt(1, nan);
      C = Pt(1, 1);
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);

      CHECK_FALSE ( isTriangleDirect(A, B, C) );
      CHECK_FALSE ( isTriangleDirect(B, C, A) );
      CHECK_FALSE ( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }

    SECTION( "Triangle avec trois sommets indéfinis" )
    {
      A = Pt(0, nan);
      B = Pt(1, nan);
      C = Pt(1, nan);
      INFO("A : " << A.x << ", " << A.y);
      INFO("B : " << B.x << ", " << B.y);
      INFO("C : " << C.x << ", " << C.y);

      CHECK_FALSE ( isTriangleDirect(A, B, C) );
      CHECK_FALSE ( isTriangleDirect(B, C, A) );
      CHECK_FALSE ( isTriangleDirect(C, A, B) );

      CHECK_FALSE( isTriangleDirect(A, C, B) );
      CHECK_FALSE( isTriangleDirect(C, B, A) );
      CHECK_FALSE( isTriangleDirect(B, A, C) );
    }
  }
}

