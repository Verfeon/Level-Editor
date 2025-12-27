#include "Level.h"
#include "Validator.h"
#include "Exporter.h"
#include <iostream>

int main() {
    Level level(10, 8);

    // Exemple d’édition
    level.grid.setTile(3, 3, Tile::Wall);
    level.grid.setTile(4, 3, Tile::Wall);

    level.entities.push_back({ "spawn", "player_spawn", 1, 1, {} });
    level.entities.push_back({ "enemy1", "enemy", 6, 2, {} });

    std::string error;
    if (!Validator::validate(level, error)) {
        std::cerr << "Validation échouée: " << error << std::endl;
        return 1;
    }

    Exporter::exportToJson(level, "level_01.json");
    std::cout << "Niveau exporté avec succès\n";
}
