#include "Validator.hpp"

bool Validator::validate(const Level& level, std::string& error) {
    int spawnCount = 0;

    for (const auto& e : level.entities) {
        if (e.x < 0 || e.y < 0 ||
            e.x >= level.grid.width() ||
            e.y >= level.grid.height()) {
            error = "Entité hors grille";
            return false;
        }

        if (level.grid.getTile(e.x, e.y) == Tile::Wall) {
            error = "Entité placée sur un mur";
            return false;
        }

        if (e.type == "player_spawn")
            spawnCount++;
    }

    if (spawnCount != 1) {
        error = "Il faut exactement un player_spawn";
        return false;
    }

    return true;
}
