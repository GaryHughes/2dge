#ifndef dge_game_hpp
#define dge_game_hpp

#include <SDL2/SDL.h>
#include "ecs/registry.hpp"
#include "asset_store.hpp"

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

    int m_window_width = 800;
    int m_window_height = 600;

private:

    void load_level(int level);

    bool m_is_running = false;
    bool m_is_debug = false;
    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    int millisecondsPreviousFrame = 0;
    ecs::registry m_registry;
    asset_store m_asset_store;

};

}

#endif