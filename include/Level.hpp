#pragma once

#include "Grid.hpp"
#include "Entity.hpp"
#include <vector>

struct Level {
    int version = 1;
    Grid grid;
    std::vector<Entity> entities;

    Level(int w, int h, const TileTypeRegistry& registry) : grid(w, h, registry) {}
    Level(Grid g) : grid(g) {}
    void Resize(int w, int h);
    void changeVersion(int v) { version = v; };

    static Level fromJson(const json j, const TileTypeRegistry& registry);
    json toJson(const TileTypeRegistry& registry) const;
};
