#ifndef dge_animation_system_hpp
#define dge_animation_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/animation_component.hpp"
#include "../components/sprite_component.hpp"
#include "../logger.hpp"


namespace dge 
{

class animation_system : public ecs::system
{
public:

    animation_system()
    {
        require_component<ecs::animation_component>();
        require_component<ecs::sprite_component>();
    }

    void update()
    {
        for (auto& entity: entities()) {
            auto& animation = entity.get_component<ecs::animation_component>();
            auto& sprite = entity.get_component<ecs::sprite_component>();
            animation.current_frame = ((SDL_GetTicks() - animation.start_time) * animation.frame_rate_speed / 1000) % animation.num_frames;
            sprite.src_rect.x = animation.current_frame * sprite.width;
        }
    }

};

}

#endif