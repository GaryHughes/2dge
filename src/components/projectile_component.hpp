#ifndef dge_ecs_projectile_component_h
#define dge_ecs_projectile_component_h

#include <SDL2/SDL.h>

namespace dge {
namespace ecs {

struct projectile_component
{
    bool is_friendly;
    int hit_percentage;
    int duration;
    int start_time;

    projectile_component(bool friendly = false, int hp = 0, int d = 0)
    :   is_friendly(friendly),
        hit_percentage(hp),
        duration(d),
        start_time(SDL_GetTicks())
    {
    }
};

}
}

#endif