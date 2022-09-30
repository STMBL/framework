#include "catch.hpp"
#define FP_USE_FLOAT
#define FP_USE_IOSTREAM
#include <fp_lib.hpp>

TEMPLATE_TEST_CASE("mehrere", "blupp", q8_24, q16_16, q24_8, double){
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
    //SECTION("wrap"){
    //    yalla = yalla.max();
    //    yalla += yalla.delta();
    //    //CAPTURE(yalla,yalla.data,yalla.min(),yalla.min().data,yalla.delta().data);
    //    //INFO("hello" << yalla.data);
    //    REQUIRE(yalla == yalla.min());
    //}

    SECTION("add"){
        REQUIRE( TestType(foo + bar) == TestType(4.0));
    }
    SECTION("division"){
        REQUIRE( TestType(foobar)/2 == TestType(1.5));
        REQUIRE( TestType(foobar)/3 == TestType(1.0));
    }
    SECTION("multiplication"){
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
}


TEMPLATE_TEST_CASE("q8_24 with other types", "blah", q8_24, q16_16, q24_8){
    double i = GENERATE(-1.0d,0.0d,1.0d,2.0d,3.0d,4.4342d);
    double j = GENERATE(-1.0d,1.0d,2.0d,3.0d,4.4342d);

    double delta = static_cast<double>(q8_24(0).delta());
    double delta_j = static_cast<double>(TestType(0).delta());
    
    SECTION("addition"){
        double added = static_cast<double>(q8_24(i + j));
        double added_2 = static_cast<double>(q8_24(q8_24(i) + TestType(j)));
        REQUIRE_THAT(added_2, Catch::Matchers::WithinAbs(added, delta_j));
    }
    SECTION("subtraction"){
        double subbed = static_cast<double>(q8_24(i - j));
        double subbed_2 = static_cast<double>(q8_24(q8_24(i) - TestType(j)));
        REQUIRE_THAT(subbed_2, Catch::Matchers::WithinAbs(subbed, delta_j));
    }
    SECTION("multiplication"){
        double mult = static_cast<double>(q8_24(i * j));
        double mult_2 = static_cast<double>(q8_24(q8_24(i) * TestType(j)));
        REQUIRE_THAT(mult_2, Catch::Matchers::WithinAbs(mult_2, delta_j));
    }
    SECTION("division"){
        double div = static_cast<double>(q8_24(i / j));
        double div_2 = static_cast<double>(q8_24(q8_24(i) / TestType(j)));
        REQUIRE_THAT(div_2, Catch::Matchers::WithinAbs(div, delta + delta_j));
    }
}

TEMPLATE_TEST_CASE("q16_16 with other types", "blah", q8_24, q16_16, q24_8){
    double i = GENERATE(-1.0d,0.0d,1.0d,2.0d,3.0d,4.4342d);
    double j = GENERATE(-1.0d,1.0d,2.0d,3.0d,4.4342d);

    double delta = static_cast<double>(q16_16(0).delta());
    double delta_j = static_cast<double>(TestType(0).delta());
    
    SECTION("addition"){
        double added = static_cast<double>(q16_16(i + j));
        double added_2 = static_cast<double>(q16_16(q16_16(i) + TestType(j)));
        REQUIRE_THAT(added_2, Catch::Matchers::WithinAbs(added, delta_j));
    }
    SECTION("subtraction"){
        double subbed = static_cast<double>(q16_16(i - j));
        double subbed_2 = static_cast<double>(q16_16(q16_16(i) - TestType(j)));
        REQUIRE_THAT(subbed_2, Catch::Matchers::WithinAbs(subbed, delta_j));
    }
    SECTION("multiplication"){
        double mult = static_cast<double>(q16_16(i * j));
        double mult_2 = static_cast<double>(q16_16(q16_16(i) * TestType(j)));
        REQUIRE_THAT(mult_2, Catch::Matchers::WithinAbs(mult_2, delta_j));
    }
    SECTION("division"){
        double div = static_cast<double>(q16_16(i / j));
        double div_2 = static_cast<double>(q16_16(q16_16(i) / TestType(j)));
        REQUIRE_THAT(div_2, Catch::Matchers::WithinAbs(div, delta + delta_j));
    }
}

TEMPLATE_TEST_CASE("q24_8 with other types", "blah", q8_24, q16_16, q24_8){
    double i = GENERATE(-1.0d,0.0d,1.0d,2.0d,3.0d,4.4342d);
    double j = GENERATE(-1.0d,1.0d,2.0d,3.0d,4.4342d);

    double delta = static_cast<double>(q24_8(0).delta());
    double delta_j = static_cast<double>(TestType(0).delta());
    
    SECTION("addition"){
        double added = static_cast<double>(q24_8(i + j));
        double added_2 = static_cast<double>(q24_8(q24_8(i) + TestType(j)));
        REQUIRE_THAT(added_2, Catch::Matchers::WithinAbs(added, delta_j));
    }
    SECTION("subtraction"){
        double subbed = static_cast<double>(q24_8(i - j));
        double subbed_2 = static_cast<double>(q24_8(q24_8(i) - TestType(j)));
        REQUIRE_THAT(subbed_2, Catch::Matchers::WithinAbs(subbed, delta_j));
    }
    SECTION("multiplication"){
        double mult = static_cast<double>(q24_8(i * j));
        double mult_2 = static_cast<double>(q24_8(q24_8(i) * TestType(j)));
        REQUIRE_THAT(mult_2, Catch::Matchers::WithinAbs(mult_2, delta_j));
    }
    SECTION("division"){
        double div = static_cast<double>(q24_8(i / j));
        double div_2 = static_cast<double>(q24_8(q24_8(i) / TestType(j)));
        REQUIRE_THAT(div_2, Catch::Matchers::WithinAbs(div, delta + delta_j));
    }
}

TEST_CASE( "Looped section" ) {
    for (int32_t i = -10; i <= 10; ++i) {
        DYNAMIC_SECTION( "Looped section " << i) {
            SUCCEED( "Everything is OK" );
        }
    }
}
