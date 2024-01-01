#ifndef dge_game_hpp
#define dge_game_hpp

#include <SDL2/SDL.h>
#include <sol/sol.hpp>
#include "ecs/registry.hpp"
#include "asset_store.hpp"
#include "event_bus/event_bus.hpp"

namespace dge
{

const int FPS = 60;
const int MILLISECONDS_PER_FRAME = 1000 / FPS;

class game
{
public:

    game();
    ~game();

    void initialise();
    void setup();
    void run();
    void process_input();
    void update();
    void render();
    void destroy();

    static int s_window_width;
    static int s_window_height;
    static int s_map_width;
    static int s_map_height;

private:

    bool m_is_running = false;
    bool m_is_debug = false;
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    int millisecondsPreviousFrame = 0;
    ecs::registry m_registry;
    asset_store m_asset_store;
    event_bus m_event_bus;
    SDL_Rect m_camera;
    sol::state m_lua;

};

}

#endif