#pragma once
#include "Level.hpp"
#include <string>
#include <fstream>

class Importer {
public:
    static std::ifstream searchFile();
    static Level importFromJson();
};
