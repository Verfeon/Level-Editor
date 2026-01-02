#include "Importer.hpp"
#include "TileTypeRegistry.hpp"
#include "json.hpp"
#include "nfd.h"
#include <fstream>

using json = nlohmann::json;
std::ifstream Importer::searchFile() {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_OpenDialog( "json", nullptr, &outPath );
        
    std::ifstream file(outPath);
    free(outPath);
    return file;
}

Level Importer::importFromJson() {
    std::ifstream file = searchFile();

    json j = json::parse(file);
    TileTypeRegistry registry = TileTypeRegistry::fromJson(j["tile_types"]);
    Grid grid = Grid::fromJson(j["grid"], registry);
    Level level(grid);
    const auto& entities = j["entities"];
    for (const auto& e : entities) {
        Entity entity = Entity::fromJson(e);
        level.entities.push_back(entity);
    }
    return level;
}