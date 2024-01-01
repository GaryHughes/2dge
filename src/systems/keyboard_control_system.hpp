#ifndef dge_keyboard_control_system_hpp
#define dge_keyboard_control_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../logger.hpp"
#include "../event_bus/event_bus.hpp"
#include "../events/key_pressed_event.hpp"
#include "../components/keyboard_control_component.hpp"

namespace dge 
{

class keyboard_control_system : public ecs::system
{
public:

    keyboard_control_system()
    {
        require_component<ecs::keyboard_control_component>();
        require_component<ecs::sprite_component>();
        require_component<ecs::rigid_body_component>();
    }

    void subscribe_to_events(event_bus& bus)
    {
        bus.subscribe_to_event(this, &keyboard_control_system::on_key_pressed);
    }

    void on_key_pressed(key_pressed_event& event)
    {
        // std::string key_code = std::to_string(event.symbol);
        // std::string key_symbol(1, event.symbol);
        // logger::info("key pressed event emmitted: [" + key_code + "] " + key_symbol);
    
        for (auto entity: entities()) {
            const auto keyboard_control = entity.get_component<ecs::keyboard_control_component>();
            auto& sprite = entity.get_component<ecs::sprite_component>();
            auto& rigid_body = entity.get_component<ecs::rigid_body_component>();
            switch (event.symbol) {
                case SDLK_UP:
                    rigid_body.velocity = keyboard_control.up_velocity;
                    sprite.src_rect.y = sprite.height * 0;
                    break;
                case SDLK_RIGHT:
                    rigid_body.velocity = keyboard_control.right_velocity;
                    sprite.src_rect.y = sprite.height * 1;
                    break;
                case SDLK_DOWN:
                    rigid_body.velocity = keyboard_control.down_velocity;
                    sprite.src_rect.y = sprite.height * 2;
                    break;
                case SDLK_LEFT:
                    rigid_body.velocity = keyboard_control.left_velocity;
                    sprite.src_rect.y = sprite.height * 3;
                    break;
            }
        }
    }

    void update()
    {
    }

};

}

#endif