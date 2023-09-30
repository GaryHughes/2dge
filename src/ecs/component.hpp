#ifndef dge_ecs_component_hpp
#define dge_ecs_component_hpp

#include <bitset>

namespace dge
{
namespace ecs
{

const unsigned int MAX_COMPONENTS = 32;

using signature = std::bitset<MAX_COMPONENTS>;

class base_component
{
protected:

    static int s_next_id;

};

template<typename ComponentType>
class component : base_component
{
public:

    static int id() 
    {
        static auto id = s_next_id++;
        return id;
    }

};

} // ecs
} // dge

#endif