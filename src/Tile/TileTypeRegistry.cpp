#include "TileTypeRegistry.hpp"
#include <iostream>
#include <imgui.h>


std::string TileTypeRegistry::generateUniqueName(const std::vector<TileTypeEntry>& types, const std::string& baseName) {
    std::vector<int> suffixes;
    bool base_exists = false;
    for (const auto& t : types) {
        if (t.type.name == baseName) {
            base_exists = true;
        } else if (t.type.name.rfind(baseName, 0) == 0) {
            std::string suffix = t.type.name.substr(baseName.length());
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

const TileType& TileTypeRegistry::get(const std::string& id) const {
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileTypeEntry& t) { return t.type.name == id; });
    if (it != types.end()) {
        return (*it).type;
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Returning first TileType." << std::endl;
        return types.at(0).type;
    }
}

const TileType& TileTypeRegistry::get(TileTypeIndex index) const {
    if (index.index < types.size()) {
        return types.at(index.index).type;
    } else {
        std::cerr << "TileType index " << index.index << " out of bounds. Returning first TileType." << std::endl;
        return types.at(0).type;
    }
}

const TileTypeIndex TileTypeRegistry::getIndex(const std::string& id) const {
    for (int i = 0; i < types.size(); i++) {
        if (types[i].type.name == id) {
            return TileTypeIndex(i, types[i].version);
        }
    }
    std::cerr << "TileType with id '" << id << "' not found. Returning index of first TileType." << std::endl;
    return TileTypeIndex(0, 0);
}

const std::vector<TileType> TileTypeRegistry::getAll() const {
    std::vector<TileType> allActiveTypes;
    for (const auto& entry : types) {
        if (entry.active) {
            allActiveTypes.push_back(entry.type);
        }
    }
    
    return allActiveTypes;
}

void TileTypeRegistry::add(TileType type) {
    type.name = generateUniqueName(types, type.name);
    if (!freeIndices.empty()) {
        size_t index = freeIndices.back();
        freeIndices.pop_back();
        size_t version = types[index].version + 1;
        types[index] = TileTypeEntry{std::move(type), version, true};
    } else {
        types.push_back(TileTypeEntry{std::move(type), 0, true});
    }
    size++;
}

void TileTypeRegistry::remove(const std::string& id) {
    if (size == 1) {
        std::cerr << "Cannot remove the only TileType." << std::endl;
        return;
    }
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileTypeEntry& t) { return t.type.name == id; });
    if (it != types.end()) {
        (*it).active = false;
        size--;
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Cannot remove." << std::endl;
    }
}

void TileTypeRegistry::rename(const std::string& id, std::string new_name) {
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileTypeEntry& t) { return t.type.name == id; });
    if (it != types.end()) {
        std::string unique_name = generateUniqueName(types, new_name);
        it->type.name = std::move(unique_name);
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Cannot rename." << std::endl;
    }
}

void TileTypeRegistry::changeColor(const std::string& id, ImVec4 new_color) {
    auto it = std::find_if(types.begin(), types.end(), [&id](const TileTypeEntry& t) { return t.type.name == id; });
    if (it != types.end()) {
        it->type.color = new_color;
    } else {
        std::cerr << "TileType with id '" << id << "' not found. Cannot change color." << std::endl;
    }
}

bool TileTypeRegistry::has(const std::string& id) const {
    return std::any_of(types.begin(), types.end(), [&id](const TileTypeEntry& t) { return t.type.name == id; });
}

bool TileTypeRegistry::isValid(TileTypeIndex index) const {
    if (index.index < types.size()) {
        return (types.at(index.index).active) && (types.at(index.index).version == index.version);
    } else {
        std::cerr << "TileType index " << index.index << " out of bounds. Returning false." << std::endl;
        return false;
    }
}

TileTypeRegistry TileTypeRegistry::fromJson(const json& j) {
    TileTypeRegistry registry;
    for (auto it = j.begin(); it != j.end(); it++) {
        TileType type = TileType::fromJson(it.value());
        registry.add(type);
    }
    
    return registry;
}

json TileTypeRegistry::toJson() const {
    json j;
    for (const auto& entry : types) {
        j.push_back(entry.type.toJson());
    }
    return j;
}
