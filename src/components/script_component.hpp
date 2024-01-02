#ifndef dge_ecs_script_component_h
#define dge_ecs_script_component_h

#include <sol/sol.hpp>

namespace dge {
namespace ecs {

struct script_component
{
    sol::function function;

    script_component(const sol::function& f = sol::lua_nil)
    :   function(f)
    {
    }

    ~script_component()
    {
        function = sol::function();
    }
};

}
}

#endif