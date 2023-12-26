#ifndef dge_ecs_keyboard_control_component_h
#define dge_ecs_keyboard_control_component_h

#include <glm/glm.hpp>

namespace dge {
namespace ecs {

struct keyboard_control_component
{
    glm::vec2 up_velocity;
    glm::vec2 right_velocity;
    glm::vec2 down_velocity;
    glm::vec2 left_velocity;
   
    keyboard_control_component(const glm::vec2& up = glm::vec2(0), const glm::vec2& right = glm::vec2(0), const glm::vec2& down = glm::vec2(0), const glm::vec2& left = glm::vec2(0))
    :   up_velocity(up),
        right_velocity(right),
        down_velocity(down),
        left_velocity(left)
    {
    }
};

}
}

#endif