#include "TileType.hpp"
#include <iostream>


TileType::TileType() {
    name = "default";
    color = ImVec4(0, 0, 0, 1);
}

bool TileType::operator==(const TileType& other) const {
    return name == other.name;
}

TileType TileType::fromJson(json j) {
    TileType type;
    type.name = j.at("name").get<std::string>();
    auto colorJson = j.at("color");
    type.color = ImVec4(colorJson[0].get<float>(), colorJson[1].get<float>(), colorJson[2].get<float>(), colorJson[3].get<float>());
    return type;
}

json TileType::toJson() const {
    json colorJson = json::array({color.x, color.y, color.z, color.w});
    return json{{"name", name}, {"color", colorJson}};
}