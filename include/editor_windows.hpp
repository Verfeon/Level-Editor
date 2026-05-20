#pragma once
#include "TileType.hpp"
#include "TileTypeRegistry.hpp"
#include "Level.hpp"
#include <imgui.h>

void addTileType(TileTypeRegistry& registry, std::string name);
void drawTileSetWindow(TileTypeRegistry& registry, ImGuiIO& io, TileType& drag_value);
void drawLevelWindow(TileTypeRegistry& registry, Level* level, const TileType& drag_value);
