#define SDL_MAIN_USE_CALLBACKS 1 
#include "Level.h"
#include "Validator.h"
#include "Exporter.h"
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
bool dragging = false;
Tile drag_value;
bool dragged_grid[NB_ROWS][NB_COLS] = { false };

Level level(NB_ROWS, NB_COLS);

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("ImGui + SDL3", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
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
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        int col = (event->button.x * NB_COLS) / w;
        int row = (event->button.y * NB_ROWS) / h;
        if (row >= 0 && row < NB_ROWS && col >= 0 && col < NB_COLS) {
            drag_value = static_cast<Tile>(1 - static_cast<int>(level.grid.getTile(col, row)));
        }
        dragging = true;
        return SDL_APP_CONTINUE;
    }
    if (event->type == SDL_EVENT_MOUSE_BUTTON_UP) {
        for (int row = 0; row < NB_ROWS; row++) {
            for (int col = 0; col < NB_COLS; col++) {
                dragged_grid[row][col] = false;
            }
        }
        dragging = false;
        return SDL_APP_CONTINUE;
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

    /*
    static bool my_tool_active = true;
    if (my_tool_active) {
        ImGui::Begin("My First Tool", &my_tool_active, ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open..", "Ctrl+O")) {  }
                if (ImGui::MenuItem("Save", "Ctrl+S"))   {  }
                if (ImGui::MenuItem("Close", "Ctrl+W"))  { my_tool_active = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        static ImVec4 my_color = { 0.4f, 0.7f, 0.0f, 1.0f };
        // Edit a color stored as 4 floats
        ImGui::ColorEdit4("Color", &my_color.x);

        // Generate samples and plot them
        float samples[100];
        for (int n = 0; n < 100; n++)
            samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
        ImGui::PlotLines("Samples", samples, 100);

        // Display contents in a scrolling region
        ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
        ImGui::BeginChild("Scrolling");
        for (int n = 0; n < 50; n++)
            ImGui::TextColored(my_color, "%04d: Some text", n);
        ImGui::EndChild();
        ImGui::End();
    }*/

    if (dragging) {
        float mouse_x = ImGui::GetMousePos().x;
        float mouse_y = ImGui::GetMousePos().y;
        int col = (mouse_x * NB_COLS) / w;
        int row = (mouse_y * NB_ROWS) / h;
        level.grid.setTile(col, row, drag_value);
    }

    for (int row = 0; row < NB_ROWS; row++) {
        for (int col = 0; col < NB_COLS; col++) {
            ImGui::SetNextWindowPos(ImVec2(col * (w / NB_COLS), row * (h / NB_ROWS)));
            ImGui::SetNextWindowSize(ImVec2(w / NB_COLS, h / NB_ROWS));
            char window_name[32];
            snprintf(window_name, sizeof(window_name), "Cell %d, %d", row, col);
            ImGui::Begin(window_name, nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
            if (level.grid.getTile(col, row) == Tile::Wall) {
                ImVec2 pmin = ImGui::GetWindowPos();
                ImVec2 win_size = ImGui::GetWindowSize();
                ImVec2 pmax = ImVec2(pmin.x + win_size.x, pmin.y + win_size.y);
                ImGui::GetWindowDrawList()->AddRectFilled(pmin, pmax, IM_COL32(255, 255, 255, 255));
            }
            ImGui::End();
        }
    }

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
    std::string error;
    if (!Validator::validate(level, error)) {
        std::cerr << "Validation échouée: " << error << std::endl;
    } else {
        Exporter::exportToJson(level, "level_01.json");
        std::cout << "Niveau exporté avec succès\n";
    }
}