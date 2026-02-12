#include "Grid.hpp"
#include <stdexcept>

Grid::Grid(int w, int h) : m_width(w), m_height(h),
    m_tiles(h, std::vector<Tile>(w, Tile::Floor)) {}

void Grid::setTile(int x, int y, Tile tile) {
    if (!(x < 0 || y < 0 || x >= m_width || y >= m_height)) {
        m_tiles[y][x] = tile;
    }
}

Tile Grid::getTile(int x, int y) const {
    return m_tiles[y][x];
}

int Grid::width() const { return m_width; }
int Grid::height() const { return m_height; }

const std::vector<std::vector<Tile>>& Grid::data() const {
    return m_tiles;
}
