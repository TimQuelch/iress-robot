#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include "robot.h"

using namespace robot;

using ComLine = std::pair<std::string, std::vector<std::string>>;

// NOLINTBEGIN (*cognitive-complexity)

TEST_CASE("is_valid_pos") {
    CHECK(is_valid_pos(0, 0));
    CHECK(is_valid_pos(2, 3));
    CHECK(is_valid_pos(4, 4));
    CHECK(!is_valid_pos(-1, 0));
    CHECK(!is_valid_pos(5, 0));
    CHECK(!is_valid_pos(0, -1));
    CHECK(!is_valid_pos(0, 5));
}

TEST_CASE("place") {
    std::optional<Robot> r;

    SECTION("Correct placements") {
        r = place({}, {"1", "2", "NORTH"});
        REQUIRE(r.has_value());
        CHECK(r->x == 1);
        CHECK(r->y == 2);
        CHECK(r->dir == Direction::North);

        r = place({}, {"3", "4", "SOUTH"});
        REQUIRE(r.has_value());
        CHECK(r->x == 3);
        CHECK(r->y == 4);
        CHECK(r->dir == Direction::South);
    }

    SECTION("Invalid position") {
        r = place({}, {"-1", "0", "NORTH"});
        CHECK(!r.has_value());
        r = place({}, {"5", "0", "NORTH"});
        CHECK(!r.has_value());
        r = place({}, {"0", "-10", "NORTH"});
        CHECK(!r.has_value());
        r = place({}, {"0", "10", "NORTH"});
        CHECK(!r.has_value());

        SECTION("Non-integer args") {
            r = place({}, {"one", "two", "NORTH"});
            REQUIRE(!r.has_value());
        }
    }

    SECTION("Invalid direction") {
        r = place({}, {"0", "0", "BACKWARDS"});
        CHECK(!r.has_value());
        r = place({}, {"0", "0", ""});
        CHECK(!r.has_value());
        r = place({}, {"0", "0", "NORTHWARDS"});
        CHECK(!r.has_value());
        r = place({}, {"0", "0", "SOUTHEAST"});
        CHECK(!r.has_value());
    }

    SECTION("Placing on already existing robot") {
        r = place(Robot{1, 2, Direction::East}, {"0", "0", "NORTH"});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 0);
        CHECK(r->dir == Direction::North);

        SECTION("Invalid new placement") {
            r = place(Robot{1, 2, Direction::East}, {"-1", "0", "NORTH"});
            REQUIRE(r.has_value());
            CHECK(r->x == 1);
            CHECK(r->y == 2);
            CHECK(r->dir == Direction::East);
        }
    }

    SECTION("Placing with invalid args") {
        SECTION("Empty args") {
            r = place({}, {});
            REQUIRE(!r.has_value());
        }

        SECTION("Not enough args") {
            r = place({}, {"1", "2"});
            REQUIRE(!r.has_value());
        }

        SECTION("Too many args") {
            r = place({}, {"1", "2", "NORTH", "extra", "is", "ignored"});
            REQUIRE(r.has_value());
            CHECK(r->x == 1);
            CHECK(r->y == 2);
            CHECK(r->dir == Direction::North);
        }
    }
}

TEST_CASE("move") {
    std::optional<Robot> r;

    SECTION("Correct moves") {
        r = move(Robot{0, 0, Direction::North}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 1);
        CHECK(r->dir == Direction::North);

        r = move(Robot{0, 0, Direction::East}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 1);
        CHECK(r->y == 0);
        CHECK(r->dir == Direction::East);

        r = move(Robot{1, 1, Direction::South}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 1);
        CHECK(r->y == 0);
        CHECK(r->dir == Direction::South);

        r = move(Robot{1, 1, Direction::West}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 1);
        CHECK(r->dir == Direction::West);
    }

    SECTION("Moves prevented off edge") {
        r = move(Robot{0, 0, Direction::South}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 0);
        CHECK(r->dir == Direction::South);

        r = move(Robot{0, 0, Direction::West}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 0);
        CHECK(r->dir == Direction::West);

        r = move(Robot{4, 4, Direction::North}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 4);
        CHECK(r->y == 4);
        CHECK(r->dir == Direction::North);

        r = move(Robot{4, 4, Direction::East}, {});
        REQUIRE(r.has_value());
        CHECK(r->x == 4);
        CHECK(r->y == 4);
        CHECK(r->dir == Direction::East);
    }

    SECTION("Move on empty value") {
        r = move({}, {});
        REQUIRE(!r.has_value());
    }

    SECTION("With args") {
        r = move(Robot{0, 0, Direction::North}, {"args", "are", "ignored"});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 1);
        CHECK(r->dir == Direction::North);
    }
}

TEST_CASE("left") {
    std::optional<Robot> r;

    r = left(Robot{0, 0, Direction::North}, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::West);

    r = left(r, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::South);

    r = left(r, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::East);

    r = left(r, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::North);

    SECTION("Empty value") {
        r = left({}, {});
        REQUIRE(!r.has_value());
    }

    SECTION("With args") {
        r = left(Robot{0, 0, Direction::North}, {"args", "are", "ignored"});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 0);
        CHECK(r->dir == Direction::West);
    }
}

TEST_CASE("right") {
    std::optional<Robot> r;

    r = right(Robot{0, 0, Direction::North}, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::East);

    r = right(r, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::South);

    r = right(r, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::West);

    r = right(r, {});
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::North);

    SECTION("Empty value") {
        r = right({}, {});
        REQUIRE(!r.has_value());
    }

    SECTION("With args") {
        r = right(Robot{0, 0, Direction::North}, {"args", "are", "ignored"});
        REQUIRE(r.has_value());
        CHECK(r->x == 0);
        CHECK(r->y == 0);
        CHECK(r->dir == Direction::East);
    }
}

TEST_CASE("process_commandline") {
    CHECK(process_commandline("command") == ComLine{"command", {}});
    CHECK(process_commandline("   command") == ComLine{"command", {}});
    CHECK(process_commandline("command   ") == ComLine{"command", {}});
    CHECK(process_commandline("command x") == ComLine{"command", {"x"}});
    CHECK(process_commandline("command x,y,z") == ComLine{"command", {"x", "y", "z"}});
    CHECK(process_commandline("command x , y , z") == ComLine{"command", {"x", "y", "z"}});
    CHECK(process_commandline("command x,,z") == ComLine{"command", {"x", "", "z"}});
    CHECK(process_commandline("command     x,y,z") == ComLine{"command", {"x", "y", "z"}});
}

TEST_CASE("run_simulation") {
    auto s = "PLACE 0,0,NORTH\n"
             "MOVE\n"
             "REPORT\n";
    auto is = std::istringstream{s};
    auto r = run_simulation(is);
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 1);
    CHECK(r->dir == Direction::North);

    s = "PLACE 0,0,NORTH\n"
        "LEFT\n"
        "REPORT\n";
    is = std::istringstream{s};
    r = run_simulation(is);
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::West);

    s = "PLACE 1,2,EAST\n"
        "MOVE\n"
        "MOVE\n"
        "LEFT\n"
        "MOVE\n"
        "REPORT\n";
    is = std::istringstream{s};
    r = run_simulation(is);
    REQUIRE(r.has_value());
    CHECK(r->x == 3);
    CHECK(r->y == 3);
    CHECK(r->dir == Direction::North);

    s = "PLACE -1,-1,NORTH\n"
        "MOVE\n"
        "REPORT\n";
    is = std::istringstream{s};
    r = run_simulation(is);
    REQUIRE(!r.has_value());

    s = "PLACE -1,-1,NORTH\n"
        "PLACE 0,0,SOUTH\n"
        "MOVE\n"
        "REPORT\n";
    is = std::istringstream{s};
    r = run_simulation(is);
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::South);

    s = "PLACE -1,-1,NORTH\n"
        "PLACE 0,0,NORTH\n"
        "MOVE\n"
        "MOVE\n"
        "PLACE 0,0,NORTH\n"
        "REPORT\n";
    is = std::istringstream{s};
    r = run_simulation(is);
    REQUIRE(r.has_value());
    CHECK(r->x == 0);
    CHECK(r->y == 0);
    CHECK(r->dir == Direction::North);

    s = "  PLACE    1, 2   ,   EAST\n" // valid
        "MOVE   \n"                    // valid
        "   MOVE\n"                    // valid
        "\n"                           // invalid
        "LEFT\n"                       // valid
        "NOTACOMMAND\n"                // invalid
        "MOVE args,are,ignored\n"      // valid
        "PLACE thisisn't valid\n"      // invalid
        "PLACE 5000, 0, backwards\n"   // invalid
        "REPORT\n";                    // valid
    is = std::istringstream{s};
    r = run_simulation(is);
    REQUIRE(r.has_value());
    CHECK(r->x == 3);
    CHECK(r->y == 3);
    CHECK(r->dir == Direction::North);
}

// NOLINTEND (*cognitive-complexity)

// Allow results of process_commandline to be printed nicely in test output
namespace Catch {
    template <>
    struct StringMaker<ComLine> {
        static std::string convert(ComLine const& v) { return fmt::format("{}", v); }
    };
} // namespace Catch
