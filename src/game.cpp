#include "game.hpp"
#include "logger.hpp"
#include "systems/movement_system.hpp"
#include "systems/render_system.hpp"
#include "systems/animation_system.hpp"
#include "systems/collision_system.hpp"
#include "systems/render_collider_system.hpp"
#include "systems/damage_system.hpp"
#include "systems/keyboard_control_system.hpp"
#include "systems/camera_movement_system.hpp"
#include "systems/projectile_emitter_system.hpp"
#include "systems/projectile_lifecycle_system.hpp"
#include "systems/render_text_system.hpp"
#include "systems/render_health_bar_system.hpp"
#include "systems/render_gui_system.hpp"
#include "level_loader.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_sdlrenderer2.h>

namespace dge
{

int game::s_window_width = 800;
int game::s_window_height = 600;
int game::s_map_width = 0;
int game::s_map_height = 0;

game::game()
{
}

game::~game()
{
}

void game::initialise()
{
    logger::info("initialise");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger::error("Error initialising SDL");
        return;
    }

    if (TTF_Init() != 0) {
        logger::error("TTF initialisation failed");
        return;
    }

    m_window = SDL_CreateWindow(nullptr, 
                                SDL_WINDOWPOS_CENTERED, 
                                SDL_WINDOWPOS_CENTERED, 
                                s_window_width,
                                s_window_height,
                                SDL_WINDOW_BORDERLESS);

    if (m_window == nullptr) {
        logger::error("Error creating SDL window");
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    if (m_renderer == nullptr) {
        logger::error("Error creating SDL renderer");
        return;
    }

    SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplSDL2_InitForSDLRenderer(m_window, m_renderer);
    ImGui_ImplSDLRenderer2_Init(m_renderer);

    m_camera.x = 0;
    m_camera.y = 0;
    m_camera.w = s_window_width;
    m_camera.h = s_window_height;

    m_is_running = true;
}

void game::run()
{
    setup();
    while (m_is_running) {
        process_input();
        update();
        render();
    }
}

void game::process_input()
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                m_is_running = false;
                break;

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    m_is_running = false;
                }
                if (event.key.keysym.sym == SDLK_d) {
                    m_is_debug = !m_is_debug;
                }
                m_event_bus.emit_event<key_pressed_event>(event.key.keysym.sym);
                break;
        };
    }

}

void game::setup()
{
    m_registry.add_system<movement_system>();
    m_registry.add_system<render_system>();
    m_registry.add_system<animation_system>();
    m_registry.add_system<collision_system>();
    m_registry.add_system<render_collider_system>();
    m_registry.add_system<damage_system>();
    m_registry.add_system<keyboard_control_system>();
    m_registry.add_system<camera_movement_system>();
    m_registry.add_system<projectile_emitter_system>();
    m_registry.add_system<projectile_lifecycle_system>();
    m_registry.add_system<render_text_system>();
    m_registry.add_system<render_health_bar_system>();
    m_registry.add_system<render_gui_system>();

    level_loader loader;
    m_lua.open_libraries(sol::lib::base, sol::lib::math);
    loader.load_level(m_lua, m_registry, m_asset_store, m_renderer, 1);
}

void game::update()
{
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecondsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double delta_time = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;

    millisecondsPreviousFrame = SDL_GetTicks();

    m_event_bus.reset();

    m_registry.get_system<damage_system>().subscribe_to_events(m_event_bus);
    m_registry.get_system<keyboard_control_system>().subscribe_to_events(m_event_bus);
    m_registry.get_system<projectile_emitter_system>().subscribe_to_events(m_event_bus);
    m_registry.get_system<movement_system>().subscribe_to_events(m_event_bus);

    m_registry.update();

    m_registry.get_system<movement_system>().update(delta_time);
    m_registry.get_system<animation_system>().update();
    m_registry.get_system<collision_system>().update(m_event_bus);
    m_registry.get_system<projectile_emitter_system>().update(m_registry);
    m_registry.get_system<camera_movement_system>().update(m_camera);
    m_registry.get_system<projectile_lifecycle_system>().update();
}

void game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    m_registry.get_system<render_system>().update(m_renderer, m_asset_store, m_camera);
    m_registry.get_system<render_text_system>().update(m_renderer, m_asset_store, m_camera);
    m_registry.get_system<render_health_bar_system>().update(m_renderer, m_asset_store, m_camera);

    if (m_is_debug) {
        m_registry.get_system<render_collider_system>().update(m_renderer, m_camera);
        m_registry.get_system<render_gui_system>().update(m_registry);
    }

    SDL_RenderPresent(m_renderer);
}

void game::destroy()
{  
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
    }

    if (m_window != nullptr) { 
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}

}