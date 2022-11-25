#include "catch.hpp"
#include <units.hpp>

TEST_CASE( "unit equality" ) {
    REQUIRE( Hz == -S);
    REQUIRE( V == Kg + 2 * M - A - 3 * S );
    REQUIRE( W == Kg + 2 * M - 3 * S);
    REQUIRE( F == 4 * S + 2 * A - 2 * M - Kg);
    REQUIRE( V - A == Kg + 2 * M - 3 * S - 2 * A);
    REQUIRE( V + S == Kg + 2 * M - 2 * S - A);
    REQUIRE( V + S - A == Kg + 2 * M - 2 * S - 2 * A);
    REQUIRE( W == V + A);
    REQUIRE( F == 4 * S + 2 * A - Kg - 2 * M);
    REQUIRE( C == A + S);
    REQUIRE( J == Kg + 2 * M - 2 * S);

    
    REQUIRE( N == Kg + M - 2 * S);
    REQUIRE( M - S == M - 2 * S + S);
    REQUIRE( M == M - S + S);
}

TEST_CASE( "unit unequality" ) {
    REQUIRE( Hz != S);
    REQUIRE( V != Kg + 2 * M - A - 4 * S );
    REQUIRE( W != Kg + 2 * M - 3 * S + A);
    REQUIRE( F != 4 * S + 2 * A - 3 * M - Kg);
    REQUIRE( V - A  + Cd != Kg + 2 * M - 3 * S - 2 * A);
    REQUIRE( V + S != Kg + 2 * M - 2 * S + A);
    REQUIRE( V + S - A != Kg + 2 * M - 2 * S - 2 * A + Mo);
    REQUIRE( W != V - A);
    REQUIRE( F != 4 * S + 2 * A - Kg - 3 * M);
    REQUIRE( C != A + S + Mo);
    REQUIRE( J != Kg + 2 * M + 2 * S);

    
    REQUIRE( N != Kg + M + 2 * S);
    REQUIRE( M + S != M - 2 * S + S);
    REQUIRE( M != M - 2 * S);
}

