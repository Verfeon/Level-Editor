#include "Grid.hpp"
#include "TileTypeRegistry.hpp"
#include <stdexcept>

Grid::Grid(int w, int h) : m_width(w), m_height(h),
    m_tiles(h, std::vector<TileType>(w, TileType())) {}

void Grid::setTile(int x, int y, TileType tile) {
    if (!(x < 0 || y < 0 || x >= m_width || y >= m_height)) {
        m_tiles[y][x] = tile;
    }
}

TileType Grid::getTile(int x, int y) const {
    return m_tiles[y][x];
}

int Grid::width() const { return m_width; }
int Grid::height() const { return m_height; }

const std::vector<std::vector<TileType>>& Grid::data() const {
    return m_tiles;
}

Grid Grid::fromJson(const json j, const TileTypeRegistry& registry) {
    int width = j["width"].get<int>();
    int height = j["height"].get<int>();
    Grid grid(width, height);
    const auto& tiles = j["tiles"];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            std::string tileName = tiles[y][x].get<std::string>();
            grid.setTile(x, y, registry.get(tileName));
        }
    }
    return grid;
}

json Grid::toJson() const {
    json j;
    j["width"] = m_width;
    j["height"] = m_height;
    j["tiles"] = json::array();
    for (const auto& row : m_tiles) {
        json jrow = json::array();
        for (const auto& tile : row) {
            jrow.push_back(tile.name);
        }
        j["tiles"].push_back(jrow);
    }
    return j;
}