#ifndef dge_ecs_sprite_component_h
#define dge_ecs_sprite_component_h

namespace dge {
namespace ecs {

struct sprite_component
{
    int width;
    int height;

    sprite_component(int w = 0, int h = 0)
    :   width(w),
        height(h)
    {
    }
};

}
}

#endif