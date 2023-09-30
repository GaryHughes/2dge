#include "game.hpp"
#include "logger.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>

namespace dge
{

game::game()
{
    logger::error("testing");
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
}

void game::update()
{
    int timeToWait = MILLISECONDS_PER_FRAME - (SDL_GetTicks() - millisecondsPreviousFrame);
    if (timeToWait > 0 && timeToWait <= MILLISECONDS_PER_FRAME) {
        SDL_Delay(timeToWait);
    }

    // double deltaTime = (SDL_GetTicks() - millisecondsPreviousFrame) / 1000.0;

    millisecondsPreviousFrame = SDL_GetTicks();

   
}

void game::render()
{
    SDL_SetRenderDrawColor(m_renderer, 21, 21, 21, 255);
    SDL_RenderClear(m_renderer);


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