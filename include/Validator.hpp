#pragma once
#include "Level.hpp"
#include <string>

class Validator {
public:
    static bool validate(const Level& level, std::string& error);
};
