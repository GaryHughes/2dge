#include "game.hpp"
#include "logger.hpp"
#include "components/transform_component.hpp"
#include "components/rigid_body_component.hpp"
#include "components/sprite_component.hpp"
#include "systems/movement_system.hpp"
#include "systems/render_system.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

namespace dge
{

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
                                m_window_width,
                                m_window_height,
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
                break;
        };
    }

}

void game::setup()
{
    m_registry.add_system<movement_system>();
    m_registry.add_system<render_system>();

    m_asset_store.add_texture(m_renderer, "tank-image", "../../assets/images/tank-panther-right.png");
    m_asset_store.add_texture(m_renderer, "truck-image", "../../assets/images/truck-ford-right.png");

    ecs::entity tank = m_registry.create_entity();
    tank.add_component<ecs::transform_component>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
    tank.add_component<ecs::rigid_body_component>(glm::vec2(40.0, 0.0));
    tank.add_component<ecs::sprite_component>("tank-image", 32, 32);

    ecs::entity truck = m_registry.create_entity();
    truck.add_component<ecs::transform_component>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0.0);
    truck.add_component<ecs::rigid_body_component>(glm::vec2(0.0, 50.0));
    truck.add_component<ecs::sprite_component>("truck-image", 32, 32);
}

void game::update()
{
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecondsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    double delta_time = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;

    millisecondsPreviousFrame = SDL_GetTicks();

    m_registry.get_system<movement_system>().update(delta_time);

    m_registry.update();
}

void game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);

    m_registry.get_system<render_system>().update(m_renderer, m_asset_store);

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