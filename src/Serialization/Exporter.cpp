#include "Exporter.hpp"
#include "Validator.hpp"
#include "nfd.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

std::ofstream Exporter::searchOrCreateFile() {
    nfdchar_t *outPath = NULL;
    nfdresult_t result = NFD_SaveDialog("json", nullptr, &outPath);

    if (result != NFD_OKAY || outPath == nullptr) {
        return std::ofstream();
    }

    std::string path(outPath);
    const std::string ext = ".json";
    
    if (path.length() < ext.length() || path.substr(path.length() - ext.length()) != ext) {
        path += ext;
    }

    std::ofstream file(path);
    free(outPath);
    return file;
}

void Exporter::exportToJson(const Level& level, const TileTypeRegistry& registry) {
    std::string error;
    if (!Validator::validate(level, registry, error)) {
        std::cerr << "Validation failed: " << error << std::endl;
        return;
    }

    std::cout << "Exporting level to JSON..." << std::endl;
    json j;

    j["level"] = level.toJson(registry);
    std::cout << "level -> json" << std::endl;
    j["tile_types"] = registry.toJson();
    
    std::cout << "json created" << std::endl;
    std::ofstream file = searchOrCreateFile();
    file << j.dump(4);
}