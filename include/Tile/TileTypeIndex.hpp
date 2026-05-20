#pragma once
#include <cstddef>

using json = nlohmann::json;

struct TileTypeIndex {
    size_t index;
    size_t version;

    TileTypeIndex() : index(0), version(0) {}
    explicit TileTypeIndex(size_t index, size_t version = 0) : index(index), version(version) {}

    bool operator==(const TileTypeIndex& other) const {
        return (index == other.index) && (version == other.version);
    }
};