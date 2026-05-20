#pragma once
#include "Level.hpp"
#include <string>

class Validator {
public:
    static bool validate(const Level& level, const TileTypeRegistry& registry, std::string& error);
};
