#pragma once

#include "json.hpp"
#include "TileType.hpp"
#include "TileTypeRegistry.hpp"
#include <vector>

using json = nlohmann::json;

class Grid {
public:
    Grid(int w, int h);

    void setTile(int x, int y, TileType tile);
    TileType getTile(int x, int y) const;

    int width() const;
    int height() const;

    const std::vector<std::vector<TileType>>& data() const;

    static Grid fromJson(const json j, const TileTypeRegistry& registry);
    json toJson() const;

private:
    int m_width;
    int m_height;
    std::vector<std::vector<TileType>> m_tiles;
};
