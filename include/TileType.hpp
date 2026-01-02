#pragma once

#include <string>
#include "json.hpp"

using json = nlohmann::json;

struct TileType {
    std::string name;

    TileType();
    TileType(std::string name) : name(std::move(name)) {};

    bool operator==(const TileType& other) const;

    static TileType fromJson(json j);
    json toJson() const;
};