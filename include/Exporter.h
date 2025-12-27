#pragma once
#include "Level.h"
#include <string>

class Exporter {
public:
    static void exportToJson(const Level& level, const std::string& path);
};
