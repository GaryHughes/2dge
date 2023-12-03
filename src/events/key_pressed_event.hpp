#ifndef dge_key_pressed_event_hpp
#define dge_key_pressed_event_hpp

#include "../ecs/ecs.hpp"
#include "../event_bus/event.hpp"
#include <SDL2/SDL.h>

namespace dge
{

class key_pressed_event : public event
{
public:

    SDL_Keycode symbol;

    key_pressed_event(SDL_Keycode _symbol)
    :   symbol(_symbol)
    {
    }


};

}

#endif