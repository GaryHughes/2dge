#ifndef dge_damage_system_hpp
#define dge_damage_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/box_collider_component.hpp"
#include "../logger.hpp"
#include "../event_bus/event_bus.hpp"
#include "../events/collision_event.hpp"

namespace dge 
{

class damage_system : public ecs::system
{
public:

    damage_system()
    {
        require_component<ecs::box_collider_component>();
    }

    void subscribe_to_events(event_bus& bus)
    {
        bus.subscribe_to_event(this, &damage_system::on_collision);
    }

    void on_collision(collision_event& event)
    {
        // logger::info("The damage system received an event between entities " + std::to_string(event.a.id()) + " and " + std::to_string(event.b.id()));
        // event.a.destroy();
        // event.b.destroy();
    }

    void update()
    {
    }

};

}

#endif