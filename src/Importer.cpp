#include "Importer.hpp"
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
    int width = j["grid"]["width"];
    int height = j["grid"]["height"];
    Level level(width, height);
    const auto& tiles = j["grid"]["tiles"];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            level.grid.setTile(x, y, static_cast<Tile>(tiles[y][x].get<int>()));
        }
    }
    const auto& entities = j["entities"];
    for (const auto& e : entities) {
        Entity entity;
        entity.type = e["type"].get<std::string>();
        entity.x = e["x"].get<int>();
        entity.y = e["y"].get<int>();
        level.entities.push_back(entity);
    }
    return level;
}
