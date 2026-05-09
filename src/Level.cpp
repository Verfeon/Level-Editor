#include "Level.hpp"
#include "TileTypeRegistry.hpp"

void Level::Resize(int w, int h) {
    Grid new_grid = Grid(w, h);
    int min_w = std::min(w, grid.width());
    int min_h = std::min(h, grid.height());
    for (int y = 0; y < min_h; y++) {
        for (int x = 0; x < min_w; x++) {
            new_grid.setTile(x, y, grid.getTileIndex(x, y));
        }
    }
    grid = new_grid;
}

Level Level::fromJson(const json j, const TileTypeRegistry& registry) {
    int version = j.at("version").get<int>();
    Grid grid = Grid::fromJson(j.at("grid"), registry);
    Level level(grid);
    level.version = version;

    const auto& entities_json = j.at("entities");
    for (const auto& e : entities_json) {
        Entity entity = Entity::fromJson(e);
        level.entities.push_back(entity);
    }

    return level;
}

json Level::toJson(const TileTypeRegistry& registry) const {
    json j;
    j["version"] = version;
    j["grid"] = grid.toJson(registry);
    j["entities"] = json::array();

    for (const auto& e : entities) {
        j["entities"].push_back(e.toJson());
    }

    return j;
}