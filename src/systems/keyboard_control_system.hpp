#ifndef dge_keyboard_control_system_hpp
#define dge_keyboard_control_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../logger.hpp"
#include "../event_bus/event_bus.hpp"
#include "../events/key_pressed_event.hpp"

namespace dge 
{

class keyboard_control_system : public ecs::system
{
public:

    keyboard_control_system()
    {
    }

    void subscribe_to_events(event_bus& bus)
    {
        bus.subscribe_to_event(this, &keyboard_control_system::on_key_pressed);
    }

    void on_key_pressed(key_pressed_event& event)
    {
        std::string key_code = std::to_string(event.symbol);
        std::string key_symbol(1, event.symbol);
        logger::info("key pressed event emmitted: [" + key_code + "] " + key_symbol);
    }

    void update()
    {
    }

};

}

#endif