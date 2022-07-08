#include <functional>
#include <map>

#include <fmt/format.h>

#include "robot.h"

namespace robot {

    bool is_valid_pos(int x, int y) { return x >= 0 && x < board_size && y >= 0 && y < board_size; }

    std::optional<Direction> str_to_dir(std::string_view str) {
        if (str == "NORTH") {
            return Direction::North;
        }
        if (str == "EAST") {
            return Direction::East;
        }
        if (str == "SOUTH") {
            return Direction::South;
        }
        if (str == "WEST") {
            return Direction::West;
        }
        return {}; // Return empty value if no match
    };

    Robot move_robot(Robot const& r) {
        switch (r.dir) {
            case Direction::North: return Robot{r.x, r.y + 1, r.dir};
            case Direction::East: return Robot{r.x + 1, r.y, r.dir};
            case Direction::South: return Robot{r.x, r.y - 1, r.dir};
            case Direction::West: return Robot{r.x - 1, r.y, r.dir};
        }
    };

    std::optional<Robot> place(std::optional<Robot> const& r,
                               std::vector<std::string> const& args) {
        // Return previous value if invalid args
        if (args.size() < 3) {
            return r;
        }
        // stoi can throw, would need to replace this with something noexcept for exceptionless code
        int x = 0;
        int y = 0;
        try {
            x = std::stoi(args[0]);
            y = std::stoi(args[1]);
        } catch (std::exception const& e) { return {}; }
        // Return previous value if invalid position
        if (x < 0 || x >= board_size || y < 0 || y >= board_size) {
            return r;
        }
        auto const dir = str_to_dir(args[2]);
        // Return previous value if direction invalid
        if (!dir) {
            return r;
        }
        return Robot{x, y, *dir};
    }

    std::optional<Robot> move(std::optional<Robot> const& r,
                              std::vector<std::string> const& /*unused*/) {
        if (!r) {
            return {};
        }
        auto const newr = move_robot(*r);
        return is_valid_pos(newr.x, newr.y) ? newr : r;
    }

    std::optional<Robot> left(std::optional<Robot> const& r,
                              std::vector<std::string> const& /*unused*/) {
        if (!r) {
            return {};
        }
        switch (r->dir) {
            case Direction::North: return Robot{r->x, r->y, Direction::West};
            case Direction::East: return Robot{r->x, r->y, Direction::North};
            case Direction::South: return Robot{r->x, r->y, Direction::East};
            case Direction::West: return Robot{r->x, r->y, Direction::South};
        }
    }

    std::optional<Robot> right(std::optional<Robot> const& r,
                               std::vector<std::string> const& /*unused*/) {
        if (!r) {
            return {};
        }
        switch (r->dir) {
            case Direction::North: return Robot{r->x, r->y, Direction::East};
            case Direction::East: return Robot{r->x, r->y, Direction::South};
            case Direction::South: return Robot{r->x, r->y, Direction::West};
            case Direction::West: return Robot{r->x, r->y, Direction::North};
        }
    }

    std::optional<Robot> report(std::optional<Robot> const& r,
                                std::vector<std::string> const& /*unused*/) {
        if (r) {
            fmt::print("x = {}, y = {}, direction = {}", r->x, r->y, r->dir);
        } else {
            fmt::print("Robot has not been validly placed yet");
        }
        return r;
    }

    std::map<std::string,
             std::function<std::optional<Robot>(std::optional<Robot> const&,
                                                std::vector<std::string> const&)>> const commands =
        {
            {"PLACE", place},
            {"MOVE", move},
            {"LEFT", left},
            {"RIGHT", right},
            {"REPORT", report},
    };

} // namespace robot

template <>
struct fmt::formatter<robot::Direction> : formatter<std::string_view> {
    template <typename FormatContext>
    auto format(robot::Direction d, FormatContext& ctx) {
        std::string_view s = "unknown";
        switch (d) {
            case robot::Direction::North: s = "NORTH"; break;
            case robot::Direction::East: s = "EAST"; break;
            case robot::Direction::South: s = "SOUTH"; break;
            case robot::Direction::West: s = "WEST"; break;
        }
        return formatter<std::string_view>::format(s, ctx);
    }
};
