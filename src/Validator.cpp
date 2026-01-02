#include "Validator.hpp"
#include "TileTypeRegistry.hpp"

bool Validator::validate(const Level& level, const TileTypeRegistry& registry, std::string& error) {
    for (const auto& e : level.entities) {
        if (e.x < 0 || e.y < 0 ||
            e.x >= level.grid.width() ||
            e.y >= level.grid.height()) {
            error = "Entité hors grille";
            return false;
        }
    }

    return true;
}
