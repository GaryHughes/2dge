#ifndef dge_render_collider_system_hpp
#define dge_render_collider_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/transform_component.hpp"
#include "../logger.hpp"

namespace dge 
{

class render_collider_system : public ecs::system
{
public:

    render_collider_system()
    {
        require_component<ecs::box_collider_component>();
        require_component<ecs::transform_component>();
    }

    void update(SDL_Renderer* renderer)
    {
        for (auto& entity: entities()) {
            auto& transform = entity.get_component<ecs::transform_component>();
            auto& collider = entity.get_component<ecs::box_collider_component>();

            SDL_Rect collider_rect = {
                int(transform.position.x + collider.offset.x),
                int(transform.position.y + collider.offset.y),
                int(collider.width),
                int(collider.height)
            };

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &collider_rect);
        }    
    }
};

}

#endif