#pragma once
#include "Level.hpp"
#include <string>
#include <fstream>

class Importer {
public:
    static bool searchFile(std::ifstream *outFile);
    static bool importFromJson(Level* outLevel, TileTypeRegistry& registry);
};
