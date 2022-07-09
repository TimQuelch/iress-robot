#pragma once

#include <optional>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <istream>

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

    bool is_valid_pos(int x, int y);

    std::optional<Robot> place(std::optional<Robot> const&, std::vector<std::string> const&);
    std::optional<Robot> move(std::optional<Robot> const&, std::vector<std::string> const&);
    std::optional<Robot> left(std::optional<Robot> const&, std::vector<std::string> const&);
    std::optional<Robot> right(std::optional<Robot> const&, std::vector<std::string> const&);
    std::optional<Robot> report(std::optional<Robot> const&, std::vector<std::string> const&);

    std::pair<std::string, std::vector<std::string>> process_commandline(std::string_view cl);

    std::optional<Robot> run_simulation(std::istream& in);
} // namespace robot
