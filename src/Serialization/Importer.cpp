#include "Importer.hpp"
#include "TileTypeRegistry.hpp"
#include "json.hpp"
#include "nfd.h"
#include <fstream>
#include <iostream>

using json = nlohmann::json;
bool Importer::searchFile(std::ifstream *outFile) {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( "json", nullptr, &outPath );
    
    if (result != NFD_OKAY || outPath == nullptr) {
        return false;
    }
    outFile->open(outPath);
    free(outPath);
    return true;
}

bool Importer::importFromJson(Level* outLevel, TileTypeRegistry& registry) {
    std::ifstream file;
    if (!searchFile(&file)) {
        std::cerr << "No file selected or failed to open file." << std::endl;
        return false;
    }

    json j = json::parse(file);
    registry = TileTypeRegistry::fromJson(j["tile_types"]);
    Grid grid = Grid::fromJson(j["level"]["grid"], registry);
    Level level(grid);
    const auto& entities = j["entities"];
    for (const auto& e : entities) {
        Entity entity = Entity::fromJson(e);
        level.entities.push_back(entity);
    }
    *outLevel = level;
    return true;
}