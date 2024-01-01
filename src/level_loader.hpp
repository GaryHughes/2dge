#ifndef dge_level_loader_hpp
#define dge_level_loader_hpp

#include "ecs/registry.hpp"
#include "asset_store.hpp"
#include <SDL2/SDL.h>
#include <sol/sol.hpp>

namespace dge 
{

class level_loader
{
public:
    
    level_loader();
    ~level_loader();

    void load_level(sol::state& lua, ecs::registry& registry, asset_store& assets, SDL_Renderer* renderer, int level);

};

}

#endif
