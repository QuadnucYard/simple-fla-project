#include "syntax/err.hpp"
#include "tm/parser.hpp"
#include "tm/sim.hpp"
#include "utils/file.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("tm case 0 (palindrome)", "[tm]") {
    auto text = read_text("tm/palindrome_detector_2tapes.tm");
    auto tm = fla::tm::Parser{text}.parse();

    SECTION("self validation") { REQUIRE(tm.validate_self()); }

    fla::tm::Simulator sim{tm, std::cout, std::cerr, {}};

    SECTION("input validation") { REQUIRE(sim("100A1A001").is_error()); }

    SECTION("simulation") {
        REQUIRE(sim("100010001").value() == "true");
        REQUIRE(sim("10001000").value() == "false");
    }
}

TEST_CASE("tm case 1 (abc)", "[tm]") {
    auto text = read_text("tm/case1.tm");
    auto tm = fla::tm::Parser{text}.parse();

    SECTION("self validation") { REQUIRE(tm.validate_self()); }

    fla::tm::Simulator sim{tm, std::cout, std::cerr, {}};

    SECTION("input validation") {
        REQUIRE(sim("c").is_error());
        REQUIRE(sim("illegal").is_error());
    }

    SECTION("simulation") {
        REQUIRE(sim("").value() == "");
        REQUIRE(sim("a").value() == "");
        REQUIRE(sim("b").value() == "");
        REQUIRE(sim("ab").value() == "c");
        REQUIRE(sim("abb").value() == "cc");
        REQUIRE(sim("aab").value() == "cc");
        REQUIRE(sim("aabbb").value() == "cccccc");
        REQUIRE(sim("aaaaabbb").value() == "ccccccccccccccc");
        REQUIRE(sim("ba").value() == "illegal_input");
        REQUIRE(sim("abba").value() == "illegal_input");
    }
}

TEST_CASE("tm case 2 (square)", "[tm]") {
    auto text = read_text("tm/case2.tm");
    auto tm = fla::tm::Parser{text}.parse();

    SECTION("self validation") { REQUIRE(tm.validate_self()); }

    fla::tm::Simulator sim{tm, std::cout, std::cerr, {}};

    SECTION("input validation") {
        REQUIRE(sim("2").is_error());
        REQUIRE(sim("true").is_error());
        REQUIRE(sim("false").is_error());
    }

    SECTION("simulation") {
        REQUIRE(sim("").value() == "false");
        REQUIRE(sim("1").value() == "true");
        REQUIRE(sim("11").value() == "false");
        REQUIRE(sim("111").value() == "false");
        REQUIRE(sim("1111").value() == "true");
        REQUIRE(sim("11111").value() == "false");
        REQUIRE(sim("11111111").value() == "false");
        REQUIRE(sim("111111111").value() == "true");
        REQUIRE(sim("1111111111").value() == "false");
    }
}
