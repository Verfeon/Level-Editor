#include "Entity.hpp"

Entity Entity::fromJson(json j) {
    Entity entity;
    entity.id = j.at("id").get<std::string>();
    entity.type = j.at("type").get<std::string>();
    entity.x = j.at("x").get<int>();
    entity.y = j.at("y").get<int>();
    if (j.contains("params")) {
        entity.params = j.at("params").get<std::map<std::string, std::string>>();
    }
}

json Entity::toJson() const {
    json j;
    j["id"] = id;
    j["type"] = type;
    j["x"] = x;
    j["y"] = y;
    j["params"] = params;
    return j;
}