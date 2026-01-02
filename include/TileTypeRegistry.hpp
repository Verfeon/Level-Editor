#pragma once

#include "TileType.hpp"
#include <unordered_map>

class TileTypeRegistry {
    private:
        std::unordered_map<std::string, TileType> types;
    
    public:
        TileTypeRegistry();
        
        const TileType& get(const std::string& id) const;
        const std::unordered_map<std::string, TileType>& getAll() const;
        void add(TileType type);
        void initialize();

        static TileTypeRegistry fromJson(const json& j);
        json toJson() const;
};