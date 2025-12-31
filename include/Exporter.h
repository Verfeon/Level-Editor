#pragma once
#include "Level.h"
#include <string>
#include <fstream>

class Exporter {
public:
    static std::ofstream searchOrCreateFile();
    static void exportToJson(const Level& level, const std::string& path);
    static void exportToJson(const Level& level);
};
