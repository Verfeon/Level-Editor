#pragma once
#include "Level.h"
#include <string>
#include <fstream>

class Importer {
public:
    static std::ifstream searchFile();
    static Level importFromJson();
};
