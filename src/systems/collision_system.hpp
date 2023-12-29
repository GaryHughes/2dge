#ifndef dge_collision_system_hpp
#define dge_collision_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/transform_component.hpp"
#include "../logger.hpp"
#include "../event_bus/event_bus.hpp"
#include "../events/collision_event.hpp"

namespace dge 
{

class collision_system : public ecs::system
{
public:

    collision_system()
    {
        require_component<ecs::box_collider_component>();
        require_component<ecs::transform_component>();
    }

    void update(event_bus& bus)
    {
        auto all_entities = entities();

        for (auto i = all_entities.begin(); i != all_entities.end(); ++i) {
            auto& i_collider = i->get_component<ecs::box_collider_component>();
            auto& i_transform = i->get_component<ecs::transform_component>();
            for (auto j = i; j != all_entities.end(); ++j) {
                if (*i == *j) {
                    continue;
                }
                auto& j_collider = j->get_component<ecs::box_collider_component>();
                auto& j_transform = j->get_component<ecs::transform_component>();

                bool collided = check_aabb_collision(
                    i_transform.position.x + i_collider.offset.x, 
                    i_transform.position.y + i_collider.offset.y, 
                    i_collider.width, 
                    i_collider.height,
                    j_transform.position.x + j_collider.offset.x, 
                    j_transform.position.y + j_collider.offset.y, 
                    j_collider.width, 
                    j_collider.height
                );

                if (collided) {
                    logger::info("entity " + std::to_string(i->id()) + " collided with entity " + std::to_string(j->id()));
                    bus.emit_event<collision_event>(*i, *j);
                }
            }    
        }
    }

    bool check_aabb_collision(double a_x, double a_y, double a_w, double a_h,
                              double b_x, double b_y, double b_w, double b_h)
    {
        return a_x < b_x + b_w &&
               a_x + a_w > b_x &&
               a_y < b_y + b_h &&
               a_y + a_h > b_h;
    }
};

}

#endif