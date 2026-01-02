#include "TileTypeRegistry.hpp"
#include <iostream>

TileTypeRegistry::TileTypeRegistry() {
    initialize();
}

const TileType& TileTypeRegistry::get(const std::string& id) const {
    auto it = types.find(id);
    if (it != types.end()) {
        return it->second;
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Returning default TileType." << std::endl;
        return types.at("default");
    }
}

const std::unordered_map<std::string, TileType>& TileTypeRegistry::getAll() const {
    return types;
}

void TileTypeRegistry::add(TileType type) {
    types[type.name] = std::move(type);
}

void TileTypeRegistry::initialize() {
    if (types.find("default") == types.end()) {
        TileType t = TileType("default");
        add(t);
    }
}

TileTypeRegistry TileTypeRegistry::fromJson(const json& j) {
    TileTypeRegistry registry;
    for (auto it = j.begin(); it != j.end(); ++it) {
        TileType type = TileType::fromJson(it.value());
        registry.add(type);
    }
}

json TileTypeRegistry::toJson() const {
    json j;
    for (const auto& [key, type] : types) {
        j[key] = type.toJson();
    }
    return j;
}