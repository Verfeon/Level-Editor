#include "TileType.hpp"
#include <iostream>


TileType::TileType() {
    name = "default";
}

bool TileType::operator==(const TileType& other) const {
    return name == other.name;
}

TileType TileType::fromJson(json j) {
    TileType type;
    type.name = j.at("name").get<std::string>();
    return type;
}

json TileType::toJson() const {
    return json{{"name", name}};
}