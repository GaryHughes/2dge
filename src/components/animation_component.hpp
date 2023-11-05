#ifndef dge_ecs_animation_component_h
#define dge_ecs_animation_component_h

#include <SDL2/SDL.h>

namespace dge {
namespace ecs {

struct animation_component
{
    int num_frames;
    int current_frame;
    int frame_rate_speed;
    bool should_loop;
    int start_time;

    animation_component(int n = 1, int s = 1, bool l = false)
    :   num_frames(n),
        current_frame(0),
        frame_rate_speed(s),
        should_loop(l),
        start_time(SDL_GetTicks())
    {
    }
};

}
}

#endif