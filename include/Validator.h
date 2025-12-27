#pragma once
#include "Level.h"
#include <string>

class Validator {
public:
    static bool validate(const Level& level, std::string& error);
};
