#define SDL_MAIN_USE_CALLBACKS 1 
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

Level level(NB_ROWS, NB_COLS);

void save() {
    std::string error;
    if (!Validator::validate(level, error)) {
        std::cerr << "Validation échouée: " << error << std::endl;
    } else {
        Exporter::exportToJson(level);
        std::cout << "Niveau exporté avec succès\n";
    }
}

void open() {
    level = Importer::importFromJson();
    std::cout << "Niveau importé avec succès\n";
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    
    ImGui::StyleColorsDark();
    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);
    level.entities.push_back({ "spawn", "player_spawn", 1, 1, {} });

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

    ImGuiStyle& style = ImGui::GetStyle();
    float titlebar_height = style.FramePadding.y * 2 + ImGui::GetFontSize();

    ImVec2 size(viewport->Size.x * 0.5, viewport->Size.y - titlebar_height);
    ImVec2 pos(viewport->Pos.x + size.x, viewport->Pos.y + titlebar_height);
    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::Begin("Level Editor Grid");

    ImVec2 cursor_pos = ImGui::GetCursorScreenPos();
    ImVec2 grid_size = ImGui::GetContentRegionAvail();
    float cell_width = grid_size.x / NB_COLS;
    float cell_height = grid_size.y / NB_ROWS;

    float mouse_x = ImGui::GetMousePos().x - cursor_pos.x;
    float mouse_y = ImGui::GetMousePos().y - cursor_pos.y;
    int col = (mouse_x * NB_COLS) / grid_size.x;
    int row = (mouse_y * NB_ROWS) / grid_size.y;
    bool isCellCorrect = (row >= 0 && row < NB_ROWS && col >= 0 && col < NB_COLS);
    static Tile drag_value;
    static bool dragging = false;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isCellCorrect) {
        if (row >= 0 && row < NB_ROWS && col >= 0 && col < NB_COLS) {
            drag_value = static_cast<Tile>(1 - static_cast<int>(level.grid.getTile(col, row)));
        }
        dragging = true;
    }

    if (dragging && isCellCorrect) {
        level.grid.setTile(col, row, drag_value);
    }

    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
        dragging = false;
    }

    for (int row = 0; row < NB_ROWS; row++) {
        for (int col = 0; col < NB_COLS; col++) {
            char window_name[32];
            snprintf(window_name, sizeof(window_name), "Cell %d, %d", row, col);    
            ImGui::BeginChild(window_name, ImVec2(cell_width, cell_height), ImGuiChildFlags_Borders, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);
            
            if (level.grid.getTile(col, row) == Tile::Wall) {
                ImVec2 pmin = ImGui::GetCursorScreenPos();
                ImVec2 win_size = ImGui::GetContentRegionAvail();
                ImVec2 pmax = ImVec2(pmin.x + win_size.x, pmin.y + win_size.y);
                ImGui::GetWindowDrawList()->AddRectFilled(pmin, pmax, IM_COL32(255, 255, 255, 255));
            }
            ImGui::EndChild();
            ImGui::SameLine();

        }
        ImGui::NewLine();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleVar();

    ImGui::End();

    // Rendering
    ImGui::Render();
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
    SDL_RenderPresent(renderer);

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