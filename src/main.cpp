#define SDL_MAIN_USE_CALLBACKS 1 
#include "TileTypeRegistry.hpp"
#include "Level.hpp"
#include "Validator.hpp"
#include "Exporter.hpp"
#include "Importer.hpp"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cmath>
#include <cstdio>
#include <iostream>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
const int FPS = 60;
const int FRAME_DELAY = 1000 / FPS; 
const int NB_ROWS = 10;
const int NB_COLS = 10;
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

Level* level = nullptr;
TileTypeRegistry tileTypeRegistry;

void save() {
    std::string error;
    if (!Validator::validate(*level, tileTypeRegistry, error)) {
        std::cout << "Validation échouée: " << error << std::endl;
    } else {
        Exporter::exportToJson(*level, tileTypeRegistry);
        std::cout << "Niveau exporté avec succès\n";
    }
}

void open() {
    if (!Importer::importFromJson(level, tileTypeRegistry)) {
        std::cout << "Échec de l'importation du niveau\n";
    } else {
        std::cout << "Niveau importé avec succès\n";
    }
}

void addTileType(std::string name) {
    tileTypeRegistry.add(TileType(name, TILE_COLORS[TILE_COLORS_INDEX]));
    TILE_COLORS_INDEX = (TILE_COLORS_INDEX + 1) % TILE_COLORS.size();
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Level Editor", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    addTileType("default");
    level = new Level(NB_COLS, NB_ROWS, tileTypeRegistry);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }
    ImGui_ImplSDL3_ProcessEvent(event);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);

    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));

    #pragma region MenuBar

    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, 0));
    ImGui::Begin("Menu Bar", (bool *) 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) { open(); }
                if (ImGui::MenuItem("Save", "Ctrl+S"))   { save();}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        if ((io.KeyCtrl) && ImGui::IsKeyPressed(ImGuiKey_S)) {
            save();
        }
        if ((io.KeyCtrl) && ImGui::IsKeyPressed(ImGuiKey_O)) {
            open();
        }

    ImGui::End();
    #pragma endregion

    ImGuiStyle& style = ImGui::GetStyle();
    const float titlebar_height = style.FramePadding.y * 2 + ImGui::GetFontSize();
    static TileType drag_value;

    #pragma region TileSetWindow

    ImVec2 size(viewport->Size.x * 0.5, viewport->Size.y - titlebar_height);
    ImVec2 pos(viewport->Pos.x, viewport->Pos.y + titlebar_height);
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Tile set", (bool *) 0, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(15.0f, 8.0f));
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Add", "Ctrl+A")) { addTileType("new_tile"); }
            if (ImGui::MenuItem("Remove", "Ctrl+R")) { tileTypeRegistry.remove(drag_value.name); }
            ImGui::EndMenuBar();
        }
        
        if ((io.KeyCtrl) && ImGui::IsKeyPressed(ImGuiKey_A)) {
            addTileType("new_tile");
        }
        if ((io.KeyCtrl) && ImGui::IsKeyPressed(ImGuiKey_R)) {
            tileTypeRegistry.remove(drag_value.name);
        }
        ImGui::PopStyleVar();

        static char new_name[64] = "";
        static bool renaming = false;
        std::vector<TileType> types = tileTypeRegistry.getAll();
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
                    tileTypeRegistry.rename(tileType.name, std::string(new_name));
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
                std::cout << "Clicked on color of " << tileType.name << std::endl;
                ImGui::OpenPopup("color_picker");
            }

            if (ImGui::BeginPopup("color_picker"))
            {
                float color[4] = { tileType.color.x, tileType.color.y, tileType.color.z, tileType.color.w };
                ImGui::ColorPicker4("##picker", color);
                tileTypeRegistry.changeColor(tileType.name, ImVec4(color[0], color[1], color[2], color[3]));
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::PopStyleVar();
        
    ImGui::End();
    #pragma endregion

    #pragma region LevelEditor

    size = ImVec2(viewport->Size.x * 0.5, viewport->Size.y - titlebar_height);
    pos = ImVec2(viewport->Pos.x + size.x, viewport->Pos.y + titlebar_height);
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Level Editor Grid");

        ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
        ImVec2 grid_size = ImGui::GetContentRegionAvail();
        const float cell_width = grid_size.x / NB_COLS;
        const float cell_height = grid_size.y / NB_ROWS;

        const float mouse_x = ImGui::GetMousePos().x - cursor_pos.x;
        const float mouse_y = ImGui::GetMousePos().y - cursor_pos.y;
        int col = (mouse_x * NB_COLS) / grid_size.x;
        int row = (mouse_y * NB_ROWS) / grid_size.y;
        bool isCellCorrect = (row >= 0 && row < NB_ROWS && col >= 0 && col < NB_COLS);
        static bool dragging = false;

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isCellCorrect) {
            dragging = true;
        }

        if (dragging && isCellCorrect) {
            level->grid.setTile(col, row, tileTypeRegistry.getIndex(drag_value.name));
        }

        if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            dragging = false;
        }

        for (int row = 0; row < NB_ROWS; row++) {
            for (int col = 0; col < NB_COLS; col++) {
                char window_name[32];
                snprintf(window_name, sizeof(window_name), "Cell %d, %d", row, col);    
                ImGui::BeginChild(window_name, ImVec2(cell_width, cell_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
                
                    ImVec2 pmin = ImGui::GetCursorScreenPos();
                    ImVec2 win_size = ImGui::GetContentRegionAvail();
                    ImVec2 pmax = ImVec2(pmin.x + win_size.x, pmin.y + win_size.y);
                    TileTypeIndex tileIndex = level->grid.getTileIndex(col, row);

                    if (tileTypeRegistry.isValid(tileIndex)) {  
                        ImU32 color = ImGui::GetColorU32(tileTypeRegistry.get(tileIndex).color);
                        ImGui::GetWindowDrawList()->AddRectFilled(pmin, pmax, color);
                    } else {
                        ImDrawList* draw = ImGui::GetWindowDrawList();
                        draw->PushClipRect(pmin, pmax, true);

                        float spacing = 10.0f;

                        for (float x = pmin.x - (pmax.y - pmin.y); x < pmax.x; x += spacing)
                        {
                            draw->AddLine(
                                ImVec2(x, pmin.y),
                                ImVec2(x + (pmax.y - pmin.y), pmax.y),
                                IM_COL32(255, 0, 255, 255)
                            );
                        }

                        draw->PopClipRect();
                    }

                ImGui::EndChild();
                ImGui::SameLine();
            }
            ImGui::NewLine();
        }
        ImGui::PopStyleVar();
        ImGui::PopStyleVar();

    ImGui::End();
    #pragma endregion

    #pragma region rendering
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);
    #pragma endregion

    // Cap the frame rate
    static Uint64 last_time = 0;
    Uint64 now = SDL_GetTicks();
    if (last_time != 0) {
        int frame_time = (int)(now - last_time);
        if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }
    last_time = SDL_GetTicks();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}