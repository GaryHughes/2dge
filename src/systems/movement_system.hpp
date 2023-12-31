#ifndef dge_movement_system_hpp
#define dge_movement_system_hpp

#include <glm/glm.hpp>
#include "../ecs/registry.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../logger.hpp"
#include "../event_bus/event_bus.hpp"
#include "../events/collision_event.hpp"

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

    void subscribe_to_events(event_bus& bus)
    {
        bus.subscribe_to_event(this, &movement_system::on_collision);
    }

    void on_collision(collision_event& event)
    {
        logger::info("collision event emitted: " + std::to_string(event.a.id()) + " and " + std::to_string(event.b.id()));

        if (event.a.has_group("enemies") && event.b.has_group("obstacles")) {
            on_enemy_hits_obstacle(event.a, event.b);    
        }
        else if (event.b.has_group("enemies") && event.a.has_group("obstacles")) {
            on_enemy_hits_obstacle(event.b, event.a);    
        }

    }

    void on_enemy_hits_obstacle(ecs::entity& enemy, ecs::entity& obstacle)
    {
        if (!enemy.has_component<ecs::rigid_body_component>() && enemy.has_component<ecs::sprite_component>()) {
            return;
        }  

        auto& rigid_body = enemy.get_component<ecs::rigid_body_component>();
        auto& sprite = enemy.get_component<ecs::sprite_component>();
        
        if (rigid_body.velocity.x != 0) {
            rigid_body.velocity.x *= -1;
            sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        }

        if (rigid_body.velocity.y != 0) {
            rigid_body.velocity.y *= -1;
            sprite.flip = sprite.flip == SDL_FLIP_NONE ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
        }
    }

    void update(double delta_time)
    {
        for (auto& entity: entities()) {
            auto& transform = entity.get_component<ecs::transform_component>();
            const auto rigid_body = entity.get_component<ecs::rigid_body_component>();
            transform.position.x += rigid_body.velocity.x * delta_time;
            transform.position.y += rigid_body.velocity.y * delta_time;   
    
            if (entity.has_tag("player")) {
                int padding_left = 10;
                int padding_top = 10;
                int padding_right = 50;
                int padding_bottom = 50;
                transform.position.x = transform.position.x < padding_left ? padding_left : transform.position.x;
                transform.position.x = transform.position.x > game::s_map_width - padding_right ? game::s_map_width - padding_right : transform.position.x;
                transform.position.y = transform.position.y < padding_top ? padding_top : transform.position.y; 
                transform.position.y = transform.position.y > game::s_map_height - padding_bottom ? game::s_map_height - padding_bottom : transform.position.y;
            }

            bool is_entity_outside_map = transform.position.x < 0 ||
                                         transform.position.x > game::s_map_width ||
                                         transform.position.y < 0 ||
                                         transform.position.y > game::s_map_height;

            if (is_entity_outside_map && !entity.has_tag("player")) {
                entity.destroy();    
            }
        }
    }

};

}

#endif