#ifndef dge_ecs_box_collider_component_h
#define dge_ecs_box_collider_component_h

#include <glm/glm.hpp>

namespace dge {
namespace ecs {

struct box_collider_component
{
    int width;
    int height;
    glm::vec2 offset;

    box_collider_component(int w = 0, int h = 0, glm::vec2 o = glm::vec2(0))
    :   width(w),
        height(h),
        offset(o)
    {
    }
};

}
}

#endif