//
//
//

#include <catch2/catch_all.hpp>
#include <stdexcept>
#include <string>
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
}

TEST_CASE("Money tests", "[from_string]") {
    // spdlog::set_level(spdlog::level::info);

    REQUIRE(Money::zero() == Money());

    // test positive numbers
    for (int i = 0; i < 100; ++i) {
        const auto cents = helpers::random_int(10000, 10000000);
        auto expected = Money(cents);
        auto value = Money::from_string(expected.to_string());

        // spdlog::info("cents: {}, value: {}, expected: {}", cents, value.to_string(), expected.to_string());

        REQUIRE(value == expected);
    }

    // negative numbers
    for (int i = 0; i < 100; ++i) {
        const auto cents = helpers::random_int(-1000000, -100);
        auto expected = Money(cents);
        auto value = Money::from_string(expected.to_string());

        // spdlog::info("cents: {}, value: {}, expected: {}", cents, value.to_string(), expected.to_string());

        REQUIRE(value == expected);
    }

    // spdlog::set_level(spdlog::level::critical);
}

TEST_CASE("Money tests", "[from_string][bad]") {
    REQUIRE_THROWS_AS(Money::from_string("$100.0"), std::invalid_argument);
    REQUIRE_THROWS_AS(Money::from_string("$100."), std::invalid_argument);
    REQUIRE_THROWS_AS(Money::from_string("$100.000"), std::invalid_argument);
    // REQUIRE_THROWS_AS(Money::from_string("100.00"), std::invalid_argument);
    // REQUIRE_THROWS_AS(Money::from_string("$-100.00"), std::invalid_argument);
}

TEST_CASE("Money tests", "[from_string][edge]") {
    REQUIRE(true);
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

    // spdlog::set_level(spdlog::level::info);
    auto v4 = Money::from_string("$10000123.45");

    // spdlog::info("big number: {} -> {}", v4.as_dollars(), v4.to_string());
    REQUIRE(v4.as_cents() == 1000012345);
    REQUIRE(v4.to_string() == "$10,000,123.45");
    // spdlog::set_level(spdlog::level::critical);
}
