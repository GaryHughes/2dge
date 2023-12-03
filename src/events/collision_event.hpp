#ifndef dge_collision_event_hpp
#define dge_collision_event_hpp

#include "../ecs/ecs.hpp"
#include "../event_bus/event.hpp"

namespace dge
{

class collision_event : public event
{
public:

    ecs::entity a;
    ecs::entity b;

    collision_event(ecs::entity& _a, ecs::entity& _b)
    : a(_a), b(_b) 
    {
    }


};

}

#endif