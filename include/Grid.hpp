#pragma once
#include <vector>

enum class Tile {
    Floor = 0,
    Wall  = 1
};

class Grid {
public:
    Grid(int w, int h);

    void setTile(int x, int y, Tile tile);
    Tile getTile(int x, int y) const;

    int width() const;
    int height() const;

    const std::vector<std::vector<Tile>>& data() const;

private:
    int m_width;
    int m_height;
    std::vector<std::vector<Tile>> m_tiles;
};
