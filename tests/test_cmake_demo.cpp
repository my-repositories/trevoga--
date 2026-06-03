#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <source_file.hpp>
#include <string>
#include <complex>

TEST_CASE("Test add", "[unit-test]"){
    std::cout << "RUNNING TEST" << std::endl;
    REQUIRE(42 == 2 + 40);
}
