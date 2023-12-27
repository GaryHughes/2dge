#include "game.hpp"
#include "logger.hpp"
#include "components/transform_component.hpp"
#include "components/rigid_body_component.hpp"
#include "components/sprite_component.hpp"
#include "components/animation_component.hpp"
#include "components/box_collider_component.hpp"
#include "components/keyboard_control_component.hpp"
#include "components/camera_follow_component.hpp"
#include "systems/movement_system.hpp"
#include "systems/render_system.hpp"
#include "systems/animation_system.hpp"
#include "systems/collision_system.hpp"
#include "systems/render_collider_system.hpp"
#include "systems/dameage_system.hpp"
#include "systems/keyboard_control_system.hpp"
#include "systems/camera_movement_system.hpp"
#include <iostream>
#include <fstream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <boost/algorithm/string.hpp>

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

    // SDL_DisplayMode display_mode;
    // SDL_GetCurrentDisplayMode(0, &display_mode);

    // m_window_width = display_mode.w;
    // m_window_height = display_mode.h;

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

void game::load_level(int level)
{
    m_registry.add_system<movement_system>();
    m_registry.add_system<render_system>();
    m_registry.add_system<animation_system>();
    m_registry.add_system<collision_system>();
    m_registry.add_system<render_collider_system>();
    m_registry.add_system<damage_system>();
    m_registry.add_system<keyboard_control_system>();
    m_registry.add_system<camera_movement_system>();

    m_asset_store.add_texture(m_renderer, "tank-image", "../../assets/images/tank-panther-right.png");
    m_asset_store.add_texture(m_renderer, "truck-image", "../../assets/images/truck-ford-right.png");
    m_asset_store.add_texture(m_renderer, "chopper-image", "../../assets/images/chopper-spritesheet.png");
    m_asset_store.add_texture(m_renderer, "radar-image", "../../assets/images/radar.png");

    m_asset_store.add_texture(m_renderer, "jungle-image", "../../assets/tilemaps/jungle.png");

    auto jungle_texture = m_asset_store.get_texture("jungle-image");
    SDL_Point size;
    SDL_QueryTexture(jungle_texture, NULL, NULL, &size.x, &size.y);
    
    std::string jungle_map_file = "../../assets/tilemaps/jungle.map";
    std::ifstream file(jungle_map_file);
    if (!file) {
        logger::error("unable to open file " + jungle_map_file);
        return;
    }
    
    int column = 0;
    int row = 0;
    const int tile_size = 32;
    const double tile_scale = 1.0;
    int tiles_per_row = size.x / tile_size;
    int tiles_per_column = size.y / tile_size;
    int total_tiles = tiles_per_row * tiles_per_column;

    logger::info(jungle_map_file + " contains " + std::to_string(total_tiles) +  " tiles");

    while (!file.eof()) {
        std::string line;
        if (!std::getline(file, line)) {
            break;
        }
        logger::info("read map line " + line);
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(","));
        logger::info("line contains " + std::to_string(tokens.size()) + " tokens");
        for (const auto& token : tokens) {
            if (token.size() != 2) {
                logger::error("unexpected token " + token + " does not have 2 characters");
                continue;
            }
            int x = std::atoi(token.substr(1, 1).c_str());
            int y = std::atoi(token.substr(0, 1).c_str());

            if (x >= tiles_per_row) {
                logger::error("column " + std::to_string(x) + " > tiles per row " + std::to_string(tiles_per_row));
                continue;
            }

            if (y >= tiles_per_column) {
                logger::error("row " + std::to_string(y) + " > tiles per column " + std::to_string(tiles_per_column));
                continue;
            }
        
            ecs::entity tile = m_registry.create_entity();
            tile.add_component<ecs::transform_component>(glm::vec2(column * (tile_size * tile_scale), row * (tile_size * tile_scale)), glm::vec2(tile_scale, tile_scale), 0.0);
            tile.add_component<ecs::sprite_component>("jungle-image", tile_size, tile_size, 0, false, x * tile_size, y * tile_size);

            column += 1;
        }
        column = 0;
        row += 1;
    }

    s_map_width = tiles_per_column * tile_size * tile_scale;
    s_map_height = tiles_per_row * tile_size * tile_scale;
    
    ecs::entity chopper = m_registry.create_entity();
    chopper.add_component<ecs::transform_component>(glm::vec2(10.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    chopper.add_component<ecs::rigid_body_component>(glm::vec2(0.0, 00.0));
    chopper.add_component<ecs::sprite_component>("chopper-image", tile_size, tile_size, 1);
    chopper.add_component<ecs::animation_component>(2, 15, true);
    chopper.add_component<ecs::keyboard_control_component>(glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80), glm::vec2(-80, 0));
    chopper.add_component<ecs::camera_follow_component>();

    ecs::entity radar = m_registry.create_entity();
    radar.add_component<ecs::transform_component>(glm::vec2(s_window_width - 74, 10), glm::vec2(1.0, 1.0), 0.0);
    radar.add_component<ecs::rigid_body_component>(glm::vec2(0.0, 0.0));
    radar.add_component<ecs::sprite_component>("radar-image", tile_size * 2, tile_size * 2, 2, true);
    radar.add_component<ecs::animation_component>(8, 5, true);

    ecs::entity tank = m_registry.create_entity();
    tank.add_component<ecs::transform_component>(glm::vec2(500.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.add_component<ecs::rigid_body_component>(glm::vec2(-30.0, 0.0));
    tank.add_component<ecs::sprite_component>("tank-image", tile_size, tile_size, 1);
    tank.add_component<ecs::box_collider_component>(tile_size, tile_size);

    ecs::entity truck = m_registry.create_entity();
    truck.add_component<ecs::transform_component>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    truck.add_component<ecs::rigid_body_component>(glm::vec2(20.0, 0.0));
    truck.add_component<ecs::sprite_component>("truck-image", tile_size, tile_size, 1);
    truck.add_component<ecs::box_collider_component>(tile_size, tile_size);
}

void game::setup()
{
    load_level(1);
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

    m_registry.update();

    m_registry.get_system<movement_system>().update(delta_time);
    m_registry.get_system<animation_system>().update();
    m_registry.get_system<collision_system>().update(m_event_bus);
    m_registry.get_system<camera_movement_system>().update(m_camera);
}

void game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    m_registry.get_system<render_system>().update(m_renderer, m_asset_store, m_camera);

    if (m_is_debug) {
        m_registry.get_system<render_collider_system>().update(m_renderer, m_camera);
    }

    SDL_RenderPresent(m_renderer);
}

void game::destroy()
{  
    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
    }

    if (m_window != nullptr) { 
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}

}