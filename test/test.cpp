#include <catch2/catch_test_macros.hpp>

#include "robot.h"

using namespace robot;

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

// TODO test left
// TODO test right
// TODO test report

// NOLINTEND (*cognitive-complexity)
