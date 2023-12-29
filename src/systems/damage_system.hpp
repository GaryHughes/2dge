#ifndef dge_damage_system_hpp
#define dge_damage_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/projectile_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/health_component.hpp"
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
        logger::info("collision event emitted: " + std::to_string(event.a.id()) + " and " + std::to_string(event.b.id()));

        if (event.a.has_group("projectiles") && event.b.has_tag("player")) {
            on_projectile_hits_player(event.a, event.b);    
        }

        if (event.b.has_group("projectiles") && event.a.has_tag("player")) {
            on_projectile_hits_player(event.b, event.a);
        }

        if (event.a.has_group("projectiles") && event.b.has_group("enemies")) {
            on_projectile_hits_enemy(event.a, event.b);
        }

        if (event.b.has_group("projectiles") && event.a.has_group("enemies")) {
            on_projectile_hits_enemy(event.b, event.a);
        }
    }

    void on_projectile_hits_enemy(ecs::entity projectile, ecs::entity enemy)
    {
        auto projectile_component = projectile.get_component<ecs::projectile_component>();
        if (projectile_component.is_friendly) {
            auto& health = enemy.get_component<ecs::health_component>();
            health.health_percentage -= projectile_component.hit_percentage;
            if (health.health_percentage <= 0) {
                enemy.destroy();
            }
            projectile.destroy();
        }
    }

    void on_projectile_hits_player(ecs::entity projectile, ecs::entity player)
    {
        auto projectile_component = projectile.get_component<ecs::projectile_component>();
        if (!projectile_component.is_friendly) {
            auto& health = player.get_component<ecs::health_component>();
            health.health_percentage -= projectile_component.hit_percentage;
            if (health.health_percentage <= 0) {
                player.destroy();
            }
            projectile.destroy();
        }
    }

    void update()
    {
    }

};

}

#endif