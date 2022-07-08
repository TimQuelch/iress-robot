#include <iostream>
#include <string>
#include <optional>

#include <fmt/format.h>

#include "robot.h"

int main() {
    std::optional<robot::Robot> robot = {};

    for (std::string line; std::getline(std::cin, line);) {
        // Identify command (if any)
        // auto command = ...;
        // Get args
        // auto args = ...;
        // Run command
        // robot = command(robot, args);
    }
    return 0;
}
