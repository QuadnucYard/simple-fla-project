#include "pda/parser.hpp"
#include "pda/sim.hpp"
#include "syntax/err.hpp"
#include "utils/file.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("pda case 0 (anbn)", "[pda]") {
    auto text = read_text("pda/anbn.pda");
    auto tm = fla::pda::Parser{text}.parse();

    SECTION("self validation") { REQUIRE(tm.validate_self()); }

    fla::pda::Simulator sim{tm, std::cout, std::cerr, {}};

    SECTION("input validation") { REQUIRE(sim("ccc").is_error()); }

    SECTION("simulation") {
        REQUIRE(sim("").value() == false);
        REQUIRE(sim("ab").value() == true);
        REQUIRE(sim("aaaaaa").value() == false);
        REQUIRE(sim("aaaaab").value() == false);
        REQUIRE(sim("aaaabb").value() == false);
        REQUIRE(sim("aaabbb").value() == true);
        REQUIRE(sim("aabbbb").value() == false);
        REQUIRE(sim("abbbbb").value() == false);
        REQUIRE(sim("bbbbbb").value() == false);
    }
}

TEST_CASE("pda case 1 (brace)", "[pda]") {
    auto text = read_text("pda/case.pda");
    auto tm = fla::pda::Parser{text}.parse();

    SECTION("self validation") { REQUIRE(tm.validate_self()); }

    fla::pda::Simulator sim{tm, std::cout, std::cerr, {}};

    SECTION("input validation") {
        REQUIRE(sim("()( )(())").is_error());
        REQUIRE(sim("()(a)(())").is_error());
    }

    SECTION("simulation") {
        REQUIRE(sim("((()))").value() == true);
        REQUIRE(sim("(()(())))").value() == false);
        REQUIRE(sim("(()()())").value() == true);
        REQUIRE(sim("(())(())").value() == true);
        REQUIRE(sim("(()))((())").value() == false);
    }
}
