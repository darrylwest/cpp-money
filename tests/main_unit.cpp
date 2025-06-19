//
// Created by Darryl West on 3/22/25.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() function

#include <spdlog/spdlog.h>

#include <catch2/catch_all.hpp>
#include <print>

struct MainTestSetup {

    MainTestSetup() {
        std::println("Catch2 tests");
    }

    ~MainTestSetup() {
        std::println("Catch2 tests completed");
    }

};

// put helpers here...
MainTestSetup setup;


