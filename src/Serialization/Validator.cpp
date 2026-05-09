#include "Validator.hpp"
#include "TileTypeRegistry.hpp"

bool Validator::validate(const Level& level, const TileTypeRegistry& registry, std::string& error) {
    for (const auto& e : level.entities) {
        if (e.x < 0 || e.y < 0 ||
            e.x >= level.grid.width() ||
            e.y >= level.grid.height()) {
            error = "Entity '" + e.id + "' is out of bounds at (" + std::to_string(e.x) + ", " + std::to_string(e.y) + ")";
            return false;
        }
    }

    // for (int y = 0; y < level.grid.height(); y++) {
    //     for (int x = 0; x < level.grid.width(); x++) {
    //         TileType tile = level.grid.getTile(x, y);
    //         if (!registry.has(tile.name)) {
    //             error = "Tile at (" + std::to_string(x) + ", " + std::to_string(y) + ") has unknown type '" + tile.name + "'";
    //             return false;
    //         }
    //     }
    // }

    return true;
}
