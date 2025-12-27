#include "Exporter.h"
#include <fstream>

void Exporter::exportToJson(const Level& level, const std::string& path) {
    std::ofstream file(path);

    file << "{\n";
    file << "  \"version\": " << level.version << ",\n";
    file << "  \"grid\": {\n";
    file << "    \"width\": " << level.grid.width() << ",\n";
    file << "    \"height\": " << level.grid.height() << ",\n";
    file << "    \"tiles\": [\n";

    const auto& tiles = level.grid.data();
    for (size_t y = 0; y < tiles.size(); ++y) {
        file << "      [";
        for (size_t x = 0; x < tiles[y].size(); ++x) {
            file << static_cast<int>(tiles[y][x]);
            if (x + 1 < tiles[y].size()) file << ",";
        }
        file << "]";
        if (y + 1 < tiles.size()) file << ",";
        file << "\n";
    }

    file << "    ]\n";
    file << "  },\n";
    file << "  \"entities\": [\n";

    for (size_t i = 0; i < level.entities.size(); ++i) {
        const auto& e = level.entities[i];
        file << "    { \"type\": \"" << e.type
             << "\", \"x\": " << e.x
             << ", \"y\": " << e.y << " }";
        if (i + 1 < level.entities.size()) file << ",";
        file << "\n";
    }

    file << "  ]\n";
    file << "}\n";
}
