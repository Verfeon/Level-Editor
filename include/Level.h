#pragma once
#include "Grid.h"
#include "Entity.h"
#include <vector>

struct Level {
    int version = 1;
    Grid grid;
    std::vector<Entity> entities;

    Level(int w, int h) : grid(w, h) {}
};
