#include "Exporter.h"
#include "nfd.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

std::ofstream Exporter::searchOrCreateFile() {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_SaveDialog( "json", nullptr, &outPath );
        
    std::ofstream file(outPath);
    free(outPath);
    return file;
}

void Exporter::exportToJson(const Level& level) {
    json j;
    j["version"] = level.version;
    j["grid"]["width"] = level.grid.width();
    j["grid"]["height"] = level.grid.height();
    j["grid"]["tiles"] = level.grid.data();
    j["entities"] = json::array();

    for (Entity e : level.entities) {
        json entity = {
            {"id", e.id},
            {"type", e.type},
            {"x", e.x},
            {"y", e.y}
        };
        j["entities"].push_back(entity);
    }

    std::ofstream file = searchOrCreateFile();
    file << j.dump(4);
}

void Exporter::exportToJson(const Level& level, const std::string& filename) {
    json j;
    j["version"] = level.version;
    j["grid"]["width"] = level.grid.width();
    j["grid"]["height"] = level.grid.height();
    j["grid"]["tiles"] = level.grid.data();
    j["entities"] = json::array();

    for (Entity e : level.entities) {
        json entity = {
            {"id", e.id},
            {"type", e.type},
            {"x", e.x},
            {"y", e.y}
        };
        j["entities"].push_back(entity);
    }

    std::ofstream file("../exports/" + filename);
    file << j.dump(4);
}
