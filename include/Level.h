#pragma once
#include "Grid.h"
#include "Entity.h"
#include <vector>

struct Level {
    int version = 1;
    Grid grid;
    std::vector<Entity> entities;

    Level(int w, int h) : grid(w, h) {}
    void Resize(int w, int h) {
        Grid new_grid = Grid(w, h);
        int min_w = std::min(w, grid.width());
        int min_h = std::min(h, grid.height());
        for (int y = 0; y < min_h; y++) {
            for (int x = 0; x < min_w; x++) {
                new_grid.setTile(x, y, grid.getTile(x, y));
            }
        }
        grid = new_grid;
    }
};
