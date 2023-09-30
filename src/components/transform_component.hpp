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
};

}
}

#endif