#ifndef dge_ecs_sprite_component_h
#define dge_ecs_sprite_component_h

#include <string>
#include <SDL2/SDL.h>

namespace dge {
namespace ecs {

struct sprite_component
{
    std::string asset_id;
    int width;
    int height;
    int z_index;
    SDL_Rect src_rect;
   
    sprite_component(const std::string& id = "", int w = 0, int h = 0, int z = 0, int src_x = 0, int src_y = 0)
    :   asset_id(id),
        width(w),
        height(h),
        z_index(z),
        src_rect(SDL_Rect { src_x, src_y, w, h })
    {
    }
};

}
}

#endif