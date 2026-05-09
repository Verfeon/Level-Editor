#pragma once

#include "json.hpp"
#include "TileType.hpp"
#include "TileTypeIndex.hpp"
#include "TileTypeRegistry.hpp"
#include <vector>

using json = nlohmann::json;

class Grid {
public:
    Grid(int w, int h);
    Grid(int w, int h, const TileTypeRegistry& registry);

    void setTile(int x, int y, const TileTypeIndex& tileIndex);
    TileTypeIndex getTileIndex(int x, int y) const;
    const TileType getTile(int x, int y, const TileTypeRegistry& registry) const;

    int width() const;
    int height() const;

    const std::vector<std::vector<TileTypeIndex>>& data() const;

    static Grid fromJson(const json j, const TileTypeRegistry& registry);
    json toJson(const TileTypeRegistry& registry) const;

private:
    int m_width;
    int m_height;
    std::vector<std::vector<TileTypeIndex>> m_tiles;
};
