#pragma once

#include "TileType.hpp"
#include "TileTypeIndex.hpp"

class TileTypeRegistry {
    private:
        struct TileTypeEntry {
            TileType type;
            size_t version;
            bool active;
        };
        std::vector<TileTypeEntry> types;
        std::vector<size_t> freeIndices;
        size_t size = 0;

        std::string generateUniqueName(const std::vector<TileTypeEntry>& types, const std::string& baseName);
    
    public:
        const TileType& get(const std::string& id) const;
        const TileType& get(TileTypeIndex index) const;
        const TileTypeIndex getIndex(const std::string& id) const;
        const std::vector<TileType> getAll() const;
        void add(TileType type);
        void remove(const std::string& id);
        void removeAll() { types.clear(); }
        void rename(const std::string& id, std::string new_name);
        void changeColor(const std::string& id, ImVec4 new_color);
        bool has(const std::string& id) const;
        bool isValid(TileTypeIndex index) const;

        static TileTypeRegistry fromJson(const json& j);
        json toJson() const;
};