//
// Created by Darryl West on 3/22/25.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() function

#include <catch2/catch_all.hpp>
#include <print>
#include <money/money.hpp>

struct MainTestSetup {

    MainTestSetup() {
        std::println("Catch2 tests");
        std::println("cpp-money, Version: {}", money::VERSION);
    }

    ~MainTestSetup() {
        std::println("Catch2 tests completed");
    }

};

// put helpers here...
MainTestSetup setup;


