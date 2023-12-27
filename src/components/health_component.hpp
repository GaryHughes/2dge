#ifndef dge_ecs_health_component_h
#define dge_ecs_health_component_h

namespace dge {
namespace ecs {

struct health_component
{
    int health_percentage;

    health_component(int hp = 0)
    :   health_percentage(hp)
    {
    }
};

}
}

#endif