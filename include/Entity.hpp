#pragma once
#include <string>
#include <map>

struct Entity {
    std::string id;
    std::string type;   // "player_spawn", "enemy"
    int x;
    int y;
    std::map<std::string, std::string> params;
};
