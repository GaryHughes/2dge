#ifndef dge_projectile_emitter_system_hpp
#define dge_projectile_emitter_system_hpp

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "../ecs/registry.hpp"
#include "../events/key_pressed_event.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/projectile_component.hpp"

namespace dge 
{

class projectile_emitter_system : public ecs::system
{
public:

    projectile_emitter_system()
    {
        require_component<ecs::projectile_emitter_component>();
        require_component<ecs::transform_component>();
    }

    void subscribe_to_events(event_bus& bus)
    {
        bus.subscribe_to_event(this, &projectile_emitter_system::on_key_pressed);
    }

    void on_key_pressed(key_pressed_event& event)
    {
        if (event.symbol != SDLK_SPACE) {
            return;
        }

        logger::info("space pressed");

        for (auto entity : entities()) {
            
            if (!entity.has_component<ecs::camera_follow_component>()) {
                continue;
            }

            logger::info("found entity with camera follow");

            const auto transform = entity.get_component<ecs::transform_component>();
            const auto emitter = entity.get_component<ecs::projectile_emitter_component>();
            const auto rigid_body = entity.get_component<ecs::rigid_body_component>();

            glm::vec2 projectile_position = transform.position;

            if (entity.has_component<ecs::sprite_component>()) {
                const auto sprite = entity.get_component<ecs::sprite_component>();
                projectile_position.x += transform.scale.x * sprite.width / 2;
                projectile_position.y += transform.scale.y * sprite.height / 2; 
            }

            glm::vec2 projectile_velocity;
            int direction_x = 0;
            int direction_y = 0;
            
            if (rigid_body.velocity.x > 0) {
                direction_x = +1;
            }

            if (rigid_body.velocity.x < 0) {
                direction_x = -1;
            }

            if (rigid_body.velocity.y > 0) {
                direction_y = +1;
            }

            if (rigid_body.velocity.y < 0) {
                direction_y = -1;
            }

            projectile_velocity.x = emitter.projectile_velocity.x * direction_x;
            projectile_velocity.y = emitter.projectile_velocity.y * direction_y;


            ecs::entity projectile = entity.registry->create_entity();
            projectile.group("projectiles");
            projectile.add_component<ecs::transform_component>(projectile_position, glm::vec2(1.0, 1.0), 0.0);
            projectile.add_component<ecs::rigid_body_component>(projectile_velocity);
            projectile.add_component<ecs::sprite_component>("bullet-texture", 4, 4, 4);
            projectile.add_component<ecs::box_collider_component>(4, 4);
            projectile.add_component<ecs::projectile_component>(emitter.is_friendly, emitter.hit_percentage, emitter.projectile_duration);
        }
    }

    void update(ecs::registry& registry)
    {
        for (auto& entity : entities()) {
            
            const auto transform = entity.get_component<ecs::transform_component>();
            auto& emitter = entity.get_component<ecs::projectile_emitter_component>();

            if (emitter.repeat_frequency == 0) {
                continue;
            }
            
            if ((SDL_GetTicks() - emitter.last_emission_time) > emitter.repeat_frequency) {
                glm::vec2 projectile_position = transform.position;
                if (entity.has_component<ecs::sprite_component>()) {
                    const auto sprite = entity.get_component<ecs::sprite_component>();
                    projectile_position.x += transform.scale.x * sprite.width / 2;
                    projectile_position.y += transform.scale.y * sprite.height / 2; 
                }
                else {
                    logger::error("projectile emitter " + std::to_string(entity.id()) + " does not have a sprite component");
                }
                ecs::entity projectile = registry.create_entity();
                logger::info("projectile emitter " + std::to_string(entity.id()) + " emitting projectile " + std::to_string(projectile.id()));
                projectile.group("projectiles");
                projectile.add_component<ecs::transform_component>(projectile_position, glm::vec2(1.0, 1.0), 0.0);
                projectile.add_component<ecs::rigid_body_component>(emitter.projectile_velocity);
                projectile.add_component<ecs::sprite_component>("bullet-texture", 4, 4, 4);
                projectile.add_component<ecs::box_collider_component>(4, 4);
                projectile.add_component<ecs::projectile_component>(emitter.is_friendly, emitter.hit_percentage, emitter.projectile_duration);
                emitter.last_emission_time = SDL_GetTicks();
            }
        }
    }

};

}

#endif