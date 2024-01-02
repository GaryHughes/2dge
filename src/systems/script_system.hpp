#ifndef dge_script_system_hpp
#define dge_script_system_hpp

#include <sol/sol.hpp>
#include "../ecs/registry.hpp"
#include "../components/script_component.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/animation_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../logger.hpp"

namespace dge 
{

std::tuple<double, double> get_entity_position(ecs::entity entity)
{
    if (entity.has_component<ecs::transform_component>()) {
        const auto& transform = entity.get_component<ecs::transform_component>();
        return std::make_tuple(transform.position.x, transform.position.y);
    }

    logger::error("invalid attempt to get the position of an entity that does not have a transform component");
    return std::make_tuple(0.0, 0.0);
}

void set_entity_position(ecs::entity entity, double x, double y)
{
    if (entity.has_component<ecs::transform_component>()) {
        auto& transform = entity.get_component<ecs::transform_component>();
        transform.position.x = x;
        transform.position.y = y;
    }
    else {
        logger::error("invalid attempt to set the position of an entity that does not have a transform component");
    }    
}

std::tuple<double, double> get_entity_velocity(ecs::entity entity)
{
    if (entity.has_component<ecs::rigid_body_component>()) {
        const auto& rigid_body = entity.get_component<ecs::rigid_body_component>();
        return std::make_tuple(rigid_body.velocity.x, rigid_body.velocity.y);
    }

    logger::error("invalid attempt to get the velocity of an entity that does not have a rigidbody component");
    return std::make_tuple(0.0, 0.0);
}

void set_entity_velocity(ecs::entity entity, double x, double y)
{
    if (entity.has_component<ecs::rigid_body_component>()) {
        auto& rigid_body = entity.get_component<ecs::rigid_body_component>();
        rigid_body.velocity.x = x;
        rigid_body.velocity.y = y;
    }
    else {
        logger::error("invalid attempt to set the velocity of an entity that does not have a rigid body component");
    } 
}

void set_entity_rotation(ecs::entity entity, double rotation)
{
    if (entity.has_component<ecs::transform_component>()) {
        auto& transform = entity.get_component<ecs::transform_component>();
        transform.rotation = rotation;
    }
    else {
        logger::error("invalid attempt to set the rotation of an entity that does not have a transform component");
    }    
}

void set_entity_animation_frame(ecs::entity entity, int frame)
{
    if (entity.has_component<ecs::animation_component>()) {
        auto& animation = entity.get_component<ecs::animation_component>();
        animation.current_frame = frame;
    }
    else {
        logger::error("invalid attempt to set the animation frame of an entity that does not have an animation component");
    }    
}

void set_projectile_velocity(ecs::entity entity, double x, double y)
{
    if (entity.has_component<ecs::projectile_emitter_component>()) {
        auto& emitter = entity.get_component<ecs::projectile_emitter_component>();
        emitter.projectile_velocity.x = x;
        emitter.projectile_velocity.y = y;
    }
    else {
        logger::error("invalid attempt to set the animation frame of an entity that does not have an animation component");
    }    
}

class script_system : public ecs::system
{
public:

    script_system()
    {
        require_component<ecs::script_component>();
    }

    void create_lua_bindings(sol::state& lua)
    {
        lua.new_usertype<ecs::entity>(
            "entity",
            "get_id", &ecs::entity::id,
            "destroy", &ecs::entity::destroy,
            "has_tag", &ecs::entity::has_tag,
            "belongs_to_group", &ecs::entity::has_group
        );
        lua.set_function("set_position", set_entity_position);
        lua.set_function("get_position", get_entity_position);
        lua.set_function("set_velocity", set_entity_velocity);
        lua.set_function("get_velocity", get_entity_velocity);
        lua.set_function("set_rotation", set_entity_rotation);
        lua.set_function("set_projectile_velocity", set_projectile_velocity);
        lua.set_function("set_animation_frame", set_entity_animation_frame);
    }

    void update(double delta_time, int elapsed_time)
    {
        for (auto& entity: entities()) {
            const auto script = entity.get_component<ecs::script_component>();
            script.function(entity, delta_time, elapsed_time);
        }
    }

};

}

#endif