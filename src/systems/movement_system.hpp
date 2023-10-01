#ifndef dge_movement_system_hpp
#define dge_movement_system_hpp

#include <glm/glm.hpp>
#include "../ecs/registry.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../logger.hpp"

namespace dge 
{

class movement_system : public ecs::system
{
public:

    movement_system()
    {
        require_component<ecs::transform_component>();
        require_component<ecs::rigid_body_component>();
    }

    void update(double delta_time)
    {
        for (auto& entity: entities()) {
            auto& transform = entity.get_component<ecs::transform_component>();
            const auto rigid_body = entity.get_component<ecs::rigid_body_component>();
            transform.position.x += rigid_body.velocity.x * delta_time;
            transform.position.y += rigid_body.velocity.y * delta_time;   
            // logger::info("entity " + std::to_string(entity.id()) + " position is now (" + std::to_string(transform.position.x) + ", " + std::to_string(transform.position.y));
        }
    }

};

}

#endif