#ifndef dge_ecs_projectile_emitter_component_h
#define dge_ecs_projectile_emitter_component_h

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace dge {
namespace ecs {

struct projectile_emitter_component
{
    glm::vec2 projectile_velocity;
    int repeat_frequency;
    int projectile_duration;
    int hit_percentage;
    bool is_friendly;
    int last_emission_time;

    projectile_emitter_component(const glm::vec2 pv = glm::vec2(0), int f = 0, int pd = 10000, int hp = 10, bool friendly = false)
    :   projectile_velocity(pv),
        repeat_frequency(f),
        projectile_duration(pd),
        hit_percentage(hp),
        is_friendly(friendly),
        last_emission_time(SDL_GetTicks())
    {
    }
};

}
}

#endif