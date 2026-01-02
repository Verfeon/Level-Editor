#pragma once

#include "json.hpp"
#include <string>
#include <map>

using json = nlohmann::json;

struct Entity {
    std::string id;
    std::string type;   // "player_spawn", "enemy"
    int x;
    int y;
    std::map<std::string, std::string> params;

    static Entity fromJson(json j);
    json toJson() const;
};
