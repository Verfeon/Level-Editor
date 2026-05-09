#pragma once

#include "TileType.hpp"

class TileTypeRegistry {
    private:
        std::vector<TileType> types;
    
    public:
        const TileType& get(const std::string& id) const;
        const std::vector<TileType>& getAll() const;
        void add(TileType type);
        void remove(const std::string& id);
        void removeAll() { types.clear(); }
        void rename(const std::string& id, std::string new_name);
        void changeColor(const std::string& id, ImVec4 new_color);

        static TileTypeRegistry fromJson(const json& j);
        json toJson() const;
};