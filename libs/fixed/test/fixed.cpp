#include "catch.hpp"
#define FP_USE_FLOAT
#define FP_USE_IOSTREAM
#include <fp_lib.hpp>

TEMPLATE_TEST_CASE("mehrere", "blupp", q8_24, q16_16, q24_8){
    TestType yalla = TestType(0);
    TestType foo = TestType(1.5);
    TestType bar = TestType(2.5);
    TestType foobar = TestType((uint32_t)3.0);
    TestType foobar2 = TestType((int32_t)-3.0);
    SECTION("postincrement"){
        REQUIRE(foo == TestType(1.5));
        REQUIRE(foo++ == TestType(1.5));
        REQUIRE(foo == TestType(2.5));
    }
    SECTION("postdecrement"){
        REQUIRE(foo == TestType(1.5));
        REQUIRE(foo-- == TestType(1.5));
        REQUIRE(foo == TestType(0.5));
    }
    SECTION("preincrement"){
        REQUIRE(foo == TestType(1.5));
        REQUIRE(++foo == TestType(2.5));
        REQUIRE(foo == TestType(2.5));
    }
    SECTION("predecrement"){
        REQUIRE(foo == TestType(1.5));
        REQUIRE(--foo == TestType(0.5));
        REQUIRE(foo == TestType(0.5));
    }
    SECTION("wrap"){
        yalla = yalla.max();
        yalla += yalla.delta();
        REQUIRE(yalla == yalla.min());

        yalla = yalla.min();
        yalla -= yalla.delta();
        REQUIRE(yalla == yalla.max());
    }

    SECTION("int *"){
        REQUIRE(((int32_t)yalla.min()) * ((int32_t)2) == (((int32_t)2) * (int32_t)yalla.min()));
        REQUIRE(((int32_t)2) * int32_t(yalla.min()) == int32_t(yalla.min() * ((int32_t)2)));
        REQUIRE(((int32_t)yalla.min()) * ((int32_t)2) == int32_t(yalla.min() * ((int32_t)2)));
        REQUIRE(((int32_t)2) * ((int32_t)yalla.min()) == int32_t(((int32_t)2) * yalla.min()));
    }
    SECTION("int /"){
        REQUIRE(int32_t(TestType(4) / ((int32_t)1)) == TestType(4));
        REQUIRE(int32_t(TestType(4) / ((int32_t)2)) == TestType(2));
        REQUIRE(int32_t((int32_t(4)) / TestType(1)) == TestType(4));
        REQUIRE(int32_t((int32_t(4)) / TestType(2)) == TestType(2));
    }
    SECTION("int +"){
        REQUIRE(((int32_t)yalla.max()) + ((int32_t)1) == (((int32_t)1) + (int32_t)yalla.max()));
        REQUIRE(((int32_t)1) + int32_t(yalla.max()) == int32_t(yalla.max() + ((int32_t)1)));
        REQUIRE(((int32_t)yalla.max()) + ((int32_t)1) == int32_t(yalla.max() + ((int32_t)1)));
        REQUIRE(((int32_t)1) + ((int32_t)yalla.max()) == int32_t(((int32_t)1) + yalla.max()));
    }
    SECTION("int -"){
        REQUIRE(((int32_t)yalla.min()) - ((int32_t)1) == int32_t(yalla.min() - ((int32_t)1)));
        REQUIRE(((int32_t)1) - ((int32_t)yalla.min()) == int32_t(((int32_t)1) - yalla.min()));
    }

    SECTION("addition"){
        REQUIRE(int32_t(yalla.max() + TestType(1)) == (int32_t(yalla.max())) + (int32_t(1)));
        REQUIRE( TestType(foo + bar) == TestType(4.0));
    }
    SECTION("subtraction"){
        REQUIRE(int32_t(yalla.min() - TestType(1)) == (int32_t(yalla.min())) - (int32_t(1)));
        REQUIRE( TestType(foo - bar) == TestType(-1));
    }
    SECTION("division"){
        REQUIRE( TestType(TestType(foobar)/2) == TestType(1.5));
        REQUIRE( TestType(TestType(foobar)/3) == TestType(1.0));
    }
    SECTION("multiplication"){
        REQUIRE(int32_t(yalla.min() * TestType(2)) == (int32_t(yalla.min())) * (int32_t(2)));
        REQUIRE( static_cast<TestType>(TestType(foo)*TestType(1.0)) == TestType(1.5));
        REQUIRE( static_cast<TestType>(TestType(foo)*TestType(2.0)) == TestType(3.0));
        REQUIRE( static_cast<TestType>(TestType(foo)*TestType(10.0)) == TestType(15.0));
        REQUIRE( static_cast<TestType>(TestType(foobar)*TestType(0.5)) == TestType(1.5));
        REQUIRE( static_cast<TestType>(foobar*foobar) == TestType(9.0));
    }
    SECTION("cast"){
        REQUIRE(static_cast<q8_24>(foo) == q8_24(1.5));
        REQUIRE(static_cast<q16_16>(foo) == q16_16(1.5));
        REQUIRE(static_cast<q24_8>(foo) == q24_8(1.5));
        REQUIRE(static_cast<int32_t>(foobar) == 3);
        REQUIRE(static_cast<int32_t>(foobar2) == -3);
        REQUIRE(static_cast<float>(foo) == 1.5);
        REQUIRE(static_cast<double>(foo) == 1.5);
    }
    SECTION("compare"){
        REQUIRE(TestType(2) == TestType(2));
        REQUIRE(TestType(2) == int32_t(2));
        REQUIRE(int32_t(2) == TestType(2));

        // REQUIRE(TestType(2) == q8_24(2));
        // REQUIRE(TestType(2) == q16_16(2));
        // REQUIRE(TestType(2) == q24_8(2));

        REQUIRE(TestType(2) > q8_24(-2));
        REQUIRE(TestType(2) > q16_16(-2));
        REQUIRE(TestType(2) > q24_8(-2));
        REQUIRE(TestType(2) > int32_t(-2));
        REQUIRE(int32_t(2) > TestType(-2));

        REQUIRE(TestType(-2) < q8_24(2));
        REQUIRE(TestType(-2) < q16_16(2));
        REQUIRE(TestType(-2) < q24_8(2));
        REQUIRE(TestType(-2) < int32_t(2));
        REQUIRE(int32_t(-2) < TestType(2));

        REQUIRE(yalla.max() > yalla.min());
        REQUIRE(yalla.min() < yalla.max());
        REQUIRE(yalla.max() != yalla.min());
    }
}


TEMPLATE_TEST_CASE("foo2", "blah", q8_24, q16_16, q24_8){
    auto i = GENERATE(TestType(-1),TestType(0),TestType(1));
    REQUIRE(i == i);
}

TEST_CASE( "Looped section" ) {
    for (int32_t i = -10; i <= 10; ++i) {
        DYNAMIC_SECTION( "Looped section " << i) {
            SUCCEED( "Everything is OK" );
        }
    }
}
