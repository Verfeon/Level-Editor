#include "TileTypeRegistry.hpp"
#include <iostream>
#include <imgui.h>


std::string generateUniqueName(const std::vector<TileType>& types, const std::string& baseName) {
    std::vector<int> suffixes;
    bool base_exists = false;
    for (const auto& t : types) {
        if (t.name == baseName) {
            base_exists = true;
        } else if (t.name.rfind(baseName, 0) == 0) {
            std::string suffix = t.name.substr(baseName.length());
            if (!suffix.empty() && std::all_of(suffix.begin(), suffix.end(), ::isdigit)) {
                int num = std::stoi(suffix);
                suffixes.push_back(num);
            }
        }
    }
    if (!base_exists) {
        return baseName;
    } else {
        std::sort(suffixes.begin(), suffixes.end());
        int expected = 2;
        for (int n : suffixes) {
            if (n > expected) break;
            if (n == expected) ++expected;
        }
        return baseName + std::to_string(expected);
    }
}

TileTypeRegistry::TileTypeRegistry() {
    initialize();
}

const TileType& TileTypeRegistry::get(const std::string& id) const {
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileType& t) { return t.name == id; });
    if (it != types.end()) {
        return *it;
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Returning default TileType." << std::endl;
        return types.at(0);
    }
}

const std::vector<TileType>& TileTypeRegistry::getAll() const {
    return types;
}

void TileTypeRegistry::add(TileType type) {
    type.name = generateUniqueName(types, type.name);
    types.push_back(std::move(type));
}

void TileTypeRegistry::remove(const std::string& id) {
    if (id == "default") {
        std::cerr << "Cannot remove the default TileType." << std::endl;
        return;
    }
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileType& t) { return t.name == id; });
    if (it != types.end()) {
        types.erase(it);
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Cannot remove." << std::endl;
    }
}

void TileTypeRegistry::rename(const std::string& id, std::string new_name) {
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileType& t) { return t.name == id; });
    if (it != types.end()) {
        if (id == "default") {
            std::cerr << "Cannot rename the default TileType." << std::endl;
            return;
        }
        std::string unique_name = generateUniqueName(types, new_name);
        it->name = std::move(unique_name);
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Cannot rename." << std::endl;
    }
}

void TileTypeRegistry::changeColor(const std::string& id, ImVec4 new_color) {
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileType& t) { return t.name == id; });
    if (it != types.end()) {
        it->color = new_color;
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Cannot change color." << std::endl;
    }
}

void TileTypeRegistry::initialize() {
    bool hasDefault = std::any_of(types.begin(), types.end(), [](const TileType& t) { return t.name == "default"; });
    if (!hasDefault) {
        TileType t = TileType("default", ImVec4(0, 0, 0, 255));
        add(t);
    }
}

TileTypeRegistry TileTypeRegistry::fromJson(const json& j) {
    TileTypeRegistry registry;
    for (auto it = j.begin(); it != j.end(); ++it) {
        TileType type = TileType::fromJson(it.value());
        if (type.name != "default")
        {
            registry.add(type);
        } else {
            registry.types[0] = type;
        }
    }
    
    return registry;
}

json TileTypeRegistry::toJson() const {
    json j;
    for (const auto& type : types) {
        j.push_back(type.toJson());
    }
    return j;
}
