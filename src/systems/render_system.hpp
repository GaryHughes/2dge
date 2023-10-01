#ifndef dge_render_system_hpp
#define dge_render_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"

namespace dge 
{

class render_system : public ecs::system
{
public:

    render_system()
    {
        require_component<ecs::transform_component>();
        require_component<ecs::sprite_component>();
    }

    void update(SDL_Renderer* renderer)
    {
        for (auto& entity: entities()) {
             const auto& transform = entity.get_component<ecs::transform_component>();
             const auto& sprite = entity.get_component<ecs::sprite_component>();

            SDL_Rect rect = {
                int(transform.position.x),
                int(transform.position.y),
                sprite.width,
                sprite.height
            };

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

};

}

#endif