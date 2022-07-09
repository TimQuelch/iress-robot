#include <iostream>
#include <string>
#include <optional>

#include <fmt/format.h>

#include "robot.h"

int main() {
    auto r = robot::run_simulation(std::cin);
    return 0;
}
