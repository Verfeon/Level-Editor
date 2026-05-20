#include "editor_windows.hpp"
#include "TileType.hpp"
#include "Level.hpp"
#include <imgui.h>
#include <vector>
#include <string>

const std::vector<ImVec4> TILE_COLORS = {
    ImVec4(0, 0, 0, 1),
    ImVec4(1, 1, 1, 1),
    ImVec4(1, 0, 0, 1),
    ImVec4(0, 1, 0, 1),
    ImVec4(0, 0, 1, 1),
    ImVec4(1, 1, 0, 1),
    ImVec4(1, 0, 1, 1),
    ImVec4(0, 1, 1, 1)
};
int TILE_COLORS_INDEX = 0;

void addTileType(TileTypeRegistry& registry, std::string name) {
    registry.add(TileType(name, TILE_COLORS[TILE_COLORS_INDEX]));
    TILE_COLORS_INDEX = (TILE_COLORS_INDEX + 1) % TILE_COLORS.size();
}

void drawCell(Level* level, TileTypeRegistry& registry, int row, int col, int cell_width, int cell_height)
{    
    char window_name[32];
    snprintf(window_name, sizeof(window_name), "Cell %d, %d", row, col);  
    ImGui::BeginChild(window_name, ImVec2(cell_width, cell_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
        ImVec2 pmin = ImGui::GetCursorScreenPos();
        ImVec2 win_size = ImGui::GetContentRegionAvail();
        ImVec2 pmax = ImVec2(pmin.x + win_size.x, pmin.y + win_size.y);
        TileTypeIndex tileIndex = level->grid.getTileIndex(col, row);
        
        if (registry.isValid(tileIndex)) {  
            ImU32 color = ImGui::GetColorU32(registry.get(tileIndex).color);
            ImGui::GetWindowDrawList()->AddRectFilled(pmin, pmax, color);
        } else { // if tile is invalid, draws hatching
            ImDrawList* draw = ImGui::GetWindowDrawList();

            int nb_lines = 5;
            float spacing = (pmax.x + pmax.y - pmin.x - pmin.y)/(nb_lines+1);
            for (float x = pmin.x - (pmax.y - pmin.y); x < pmax.x; x += spacing)
            {
                draw->AddLine(
                    ImVec2(x, pmin.y),
                    ImVec2(x + (pmax.y - pmin.y), pmax.y),
                    IM_COL32(255, 0, 255, 255)
                );
            }
        }
    ImGui::EndChild();
}

void drawLevelWindow(TileTypeRegistry& registry, Level* level, const TileType& drag_value) 
{    
    ImGui::Begin("Level Editor Grid");
        ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
        ImVec2 grid_size = ImGui::GetContentRegionAvail();
        const int nb_cols = level->grid.width();
        const int nb_rows = level->grid.height();
        const float cell_width = grid_size.x / nb_cols;
        const float cell_height = grid_size.y / nb_rows;
        const float mouse_x = ImGui::GetMousePos().x - cursor_pos.x;
        const float mouse_y = ImGui::GetMousePos().y - cursor_pos.y;
        int col = (mouse_x * nb_cols) / grid_size.x;
        int row = (mouse_y * nb_rows) / grid_size.y;
        bool isCellCorrect = (row >= 0 && row < nb_rows && col >= 0 && col < nb_cols);
        static bool dragging = false;

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isCellCorrect) {
            dragging = true;
        }
        if (dragging && isCellCorrect) {
            level->grid.setTile(col, row, registry.getIndex(drag_value.name));
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            dragging = false;
        }

        for (int row = 0; row < nb_rows; row++) {
            for (int col = 0; col < nb_cols; col++) {
                drawCell(level, registry, row, col, cell_width, cell_height);
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();
    ImGui::End();
}

void drawTileSetWindow(TileTypeRegistry& registry, ImGuiIO& io, TileType& drag_value)
{
    ImGui::Begin("Tile set", (bool *) 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15.0f, 8.0f));
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Add", "Ctrl+A")) { addTileType(registry, "new_tile"); }
            if (ImGui::MenuItem("Remove", "Ctrl+R")) { registry.remove(drag_value.name); }
            ImGui::EndMenuBar();
        }
        if ((io.KeyCtrl) && ImGui::IsKeyPressed(ImGuiKey_A)) {
            addTileType(registry, "new_tile");
        }
        if ((io.KeyCtrl) && ImGui::IsKeyPressed(ImGuiKey_R)) {
            registry.remove(drag_value.name);
        }
        ImGui::PopStyleVar();
        static char new_name[64] = "";
        static bool renaming = false;
        std::vector<TileType> types = registry.getAll();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));
        for (const auto& tileType : types) {
            bool isSelected = (tileType.name == drag_value.name);
            ImGui::PushID(tileType.name.c_str());
            ImGui::Selectable(("  "  + tileType.name).c_str(), isSelected, ImGuiSelectableFlags_AllowDoubleClick | ImGuiSelectableFlags_AllowOverlap);
            if (ImGui::IsItemClicked()) {
                drag_value = tileType;
            }
            if (renaming && isSelected) {
                ImGui::SetKeyboardFocusHere();
                if (ImGui::InputText("##edit_tile_name", new_name, sizeof(new_name), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
                    registry.rename(tileType.name, std::string(new_name));
                    renaming = false;
                }
                if (!ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                    renaming = false;
                }
            } else {
                if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                    renaming = true;
                }
            }
            ImVec2 min = ImGui::GetItemRectMin();
            ImVec2 max = ImGui::GetItemRectMax();
            float size = (max.y - min.y) * 0.6f;
            ImVec2 square_min = ImVec2(min.x + 5.0f, min.y + (max.y - min.y - size) * 0.5f);
            ImVec2 square_max = ImVec2(square_min.x + size, square_min.y + size);
            ImDrawList* draw = ImGui::GetWindowDrawList();
            ImU32 col = ImGui::ColorConvertFloat4ToU32(tileType.color);
            draw->AddRectFilled(square_min, square_max, col);
            draw->AddRect(square_min, square_max, IM_COL32(0,0,0,255)); // bordure
            ImGui::SetCursorScreenPos(square_min);
            ImGui::InvisibleButton("color_btn", ImVec2(size, size));
            if (ImGui::IsItemClicked())
            {
                ImGui::OpenPopup("color_picker");
            }
            if (ImGui::BeginPopup("color_picker"))
            {
                float color[4] = { tileType.color.x, tileType.color.y, tileType.color.z, tileType.color.w };
                ImGui::ColorPicker4("##picker", color);
                registry.changeColor(tileType.name, ImVec4(color[0], color[1], color[2], color[3]));
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::PopStyleVar();
    ImGui::End();
}
