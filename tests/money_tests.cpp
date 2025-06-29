//
//
//

#include <catch2/catch_all.hpp>
#include <stdexcept>
#include <string>
#include <print>
#include <random>
#include <money/money.hpp>

using namespace money;

namespace helpers {
    std::random_device random_device;  // Obtain a random number from hardware
    std::mt19937 generator(random_device());

    auto random_int(int min = 1'000'000, int max = 9'999'999) {
        std::uniform_int_distribution<int> distr(min, max);
        return distr(generator);
    }

    double random_double(float min = -200000.0, float max = 2000000.0) {
        std::uniform_real_distribution<double> distr(min, max);
        return distr(generator);
    }
}

TEST_CASE("Money tests", "[constructor]") {
    for (int i = 0; i < 100; ++i) {
        const auto cents = helpers::random_int(-100000000, 100000000);
        auto value = Money(cents);
        REQUIRE(value.as_cents() == cents);

        const auto one = Money(1);
        const auto minus_one = Money(-1);

        auto v1 = Money(cents + 1);
        REQUIRE(v1 > value);
        REQUIRE(v1 - one == value);
        REQUIRE(v1 + minus_one == value);
        auto v2 = Money(cents - 1);
        REQUIRE(v2 < value);
        REQUIRE(v2 + one == value);
        REQUIRE(v2 - minus_one == value);

        auto v3 = value * 3;
        REQUIRE(v3.as_cents() == cents * 3);

        auto v4 = Money(cents / 2);
        auto v5 = Money::zero();
        v5 += v4;

        REQUIRE(v5 == v4);

    }

    auto v1 = Money(12345, 99);
    REQUIRE(v1.as_cents() == 1234599);

    auto v2 = Money(-12345, 99);
    REQUIRE(v2.as_cents() == -1234599);

    auto v3 = Money(-1, 50);
    REQUIRE(v3.as_cents() == -150);

    REQUIRE_THROWS_AS(Money(7, 101), std::invalid_argument);
}

TEST_CASE("Money tests", "[from_string]") {
    // test positive numbers
    for (int i = 0; i < 100; ++i) {
        const auto cents = helpers::random_int(1000, 10000000);
        auto expected = Money(cents);
        auto value = Money::from_string(expected.to_string());

        // std::println("cents: {}, value: {}, expected: {}", cents, value.to_string(), expected.to_string());

        REQUIRE(value == expected);
    }

    // negative numbers
    for (int i = 0; i < 100; ++i) {
        const auto cents = helpers::random_int(-10000000, -1000);
        auto expected = Money(cents);
        auto value = Money::from_string(expected.to_string());

        // std::println("cents: {}, value: {}, expected: {}", cents, value.to_string(), expected.to_string());

        REQUIRE(value == expected);
    }
}

TEST_CASE("Money tests", "[dollars,cents]") {
    int64_t dollars = 12345;
    int8_t cents = 0;
    auto v1 = Money(dollars, cents);
    REQUIRE(v1.cents() == cents);
    REQUIRE(v1.dollars() == dollars);
    auto v2 = Money(1234500);
    REQUIRE(v1.as_cents() == 1234500);
    REQUIRE(v1 == v2);

    v1 = Money(-1234567);
    REQUIRE(v1.cents() == -67);
    REQUIRE(v1.dollars() == -12345);
}

TEST_CASE("Money tests", "[from_string][bad]") {
    REQUIRE_THROWS_AS(Money::from_string("$100.0"), std::invalid_argument);
    REQUIRE_THROWS_AS(Money::from_string("$100."), std::invalid_argument);
    REQUIRE_THROWS_AS(Money::from_string("$100.000"), std::invalid_argument);
    // REQUIRE_THROWS_AS(Money::from_string("100.00"), std::invalid_argument);
    // REQUIRE_THROWS_AS(Money::from_string("$-100.00"), std::invalid_argument);
}

TEST_CASE("Money tests", "[from_string][edge]") {
    REQUIRE(Money::zero() == Money());

    auto v1 = Money::from_string("$0.00");
    REQUIRE(v1 == Money::zero());
    REQUIRE(v1.as_cents() == 0);
    REQUIRE(v1.as_dollars() == 0.0);

    auto v2 = Money::from_string("-$0.01");
    REQUIRE(v2 < Money::zero());
    REQUIRE(v2.as_cents() == -1);
    REQUIRE(v2.as_dollars() == -0.01);

    auto v3 = Money::from_string("$0.01");
    REQUIRE(v3 > Money::zero());
    REQUIRE(v3.as_cents() == 1);
    REQUIRE(v3.as_dollars() == 0.01);

    REQUIRE(v2 + v3 == Money::zero());

    auto v4 = Money::from_string("$10000123.45");
    auto v5 = Money::from_dollars(10000123.45);
    // std::println("big numbers: {} -> {}", v4.as_cents(), v5.as_cents());

    // std::println("big number: {} -> {}", v4.as_dollars(), v4.to_string());
    REQUIRE(v4.as_cents() == 1000012345);
    REQUIRE(v4.to_string() == "$10,000,123.45");

    auto v6 = Money::from_string("$1,234.56");
    REQUIRE(v6.as_cents() == 123456);

    try {
        v4 = Money::from_string("-$70123.45");
        v5 = Money::from_dollars(-70123.45);
        // std::println("big numbers: {} -> {}", v4.as_cents(), v5.as_cents());
        REQUIRE(v4 == v5);
    } catch (const std::invalid_argument& e) {
        std::println("big numbers test failed on negative numbers");
        REQUIRE(false);
    }
}

TEST_CASE("Money tests", "[from_dollars]") {
    double dollars = 12345.01;
    auto v1 = Money::from_dollars(dollars);
    REQUIRE(v1.as_cents() == 1234501);

    dollars = 5432.0;
    v1 = Money::from_dollars(dollars);
    REQUIRE(v1.as_cents() == 543200);

    dollars = -123.33;
    v1 = Money::from_dollars(dollars);
    REQUIRE(v1.as_cents() == -12333);

    dollars = -123.00;
    v1 = Money::from_dollars(dollars);
    REQUIRE(v1.as_cents() == -12300);
}

TEST_CASE("Money tests", "[ops]") {
    const auto p1 = Money::from_dollars(342.45);
    REQUIRE(p1.as_cents() == 34245);

    const auto p2 = Money(34245);
    REQUIRE(p2 == p1);

    REQUIRE(p1 + p2 == p1 * 2);
    REQUIRE(p1 - p2 == Money::zero());

    const auto p3 = Money::from_dollars(100);
    REQUIRE(p3 / 2 == Money(5000));

    const auto p4 = Money(100);
    auto scale = 1.1;
    const auto p5 = p4 * scale;
    REQUIRE(p5.as_cents() == 110.00);

    scale = 0.95;
    const auto p6 = p4 * scale;
    REQUIRE(p6.as_cents() == 95.00);
}

TEST_CASE("Money tests", "[from_dollars][zero]") {
    auto v1 = Money::from_dollars(0.0);
    REQUIRE(v1.as_cents() == 0);
    REQUIRE(v1.as_dollars() == 0.0);

    auto v2 = Money::from_dollars(0.00);
    REQUIRE(v2.as_cents() == 0);
    REQUIRE(v2.as_dollars() == 0.0);
}

TEST_CASE("Money tests", "[multiplication][negative_scalar]") {
    const auto p7 = Money(100);
    const auto p8 = p7 * -1;
    REQUIRE(p8.as_cents() == -100);
}
