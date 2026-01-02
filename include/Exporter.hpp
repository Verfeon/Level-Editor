#pragma once
#include "Level.hpp"
#include "TileTypeRegistry.hpp"
#include <string>
#include <fstream>

class Exporter {
public:
    static std::ofstream searchOrCreateFile();
    static void exportToJson(const Level& level, const TileTypeRegistry& registry);
    static json toJson(const Level& level);
};
