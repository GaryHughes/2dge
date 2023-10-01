#ifndef dge_ecs_transform_component_h
#define dge_ecs_transform_component_h

#include <glm/glm.hpp>

namespace dge {
namespace ecs {

struct transform_component
{
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;

    transform_component(const glm::vec2& p = glm::vec2(0, 0), const glm::vec2& s = glm::vec2(1, 1), double r = 0.0)
    :   position(p),
        scale(s),
        rotation(r)
    {
    }
};

}
}

#endif