#pragma once

#include <istream>
#include <optional>
#include <string>
#include <vector>

namespace robot {
    constexpr auto board_size = 5;

    enum class Direction {
        North,
        South,
        East,
        West,
    };

    struct Robot {
        int x = 0;
        int y = 0;
        Direction dir = Direction::North;
    };

    // Check if a position is valid on the board
    bool is_valid_pos(int x, int y);

    // Place robot on board command. Takes (x, y, direction) args.
    // Fails if:
    // - Insufficient args
    // - Fail to convert the args to correct types
    // - Invalid position/direction
    // On fail returns the input robot if present or empty value
    std::optional<Robot> place(std::optional<Robot> const&, std::vector<std::string> const&);

    // Move robot in stored direction. Takes no action if resulting move would be invalid. Fails if
    // input robot is empty
    std::optional<Robot> move(std::optional<Robot> const&, std::vector<std::string> const&);

    // Move robot in stored direction. Takes no action if resulting move would be invalid. Fails if
    // input robot is empty
    std::optional<Robot> left(std::optional<Robot> const&, std::vector<std::string> const&);

    // Turn robot left. Fails if input robot is empty
    std::optional<Robot> right(std::optional<Robot> const&, std::vector<std::string> const&);

    // Turn robot right. Fails if input robot is empty
    std::optional<Robot> report(std::optional<Robot> const&, std::vector<std::string> const&);

    // Process a commandline (command x, y, z) into a command/args pair. Spaces are removed around
    // command and each argument. Args may be empty
    std::pair<std::string, std::vector<std::string>> process_commandline(std::string_view cl);

    // Run the simulation line-by-line on a input stream. Use run_simulation(std::cin) to run from
    // stdin. String stream or file stream can be used for testing.
    std::optional<Robot> run_simulation(std::istream& in);
} // namespace robot
