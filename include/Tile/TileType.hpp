#pragma once

#include <string>
#include <imgui.h>
#include "json.hpp"

using json = nlohmann::json;

struct TileType {
    std::string name;
    ImVec4 color;
    bool isActive = true;

    TileType();
    TileType(std::string name, ImVec4 color) : name(std::move(name)), color(color) {};

    bool operator==(const TileType& other) const;

    static TileType fromJson(json j);
    json toJson() const;
};