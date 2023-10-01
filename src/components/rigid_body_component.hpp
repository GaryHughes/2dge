#ifndef dge_ecs_rigid_body_component_h
#define dge_ecs_rigid_body_component_h

#include <glm/glm.hpp>

namespace dge {
namespace ecs {

struct rigid_body_component
{
    glm::vec2 velocity;

    rigid_body_component(const glm::vec2& v = glm::vec2(0.0, 0.0))
    :   velocity(v)
    {
    }
};

}
}

#endif