#ifndef dge_projectile_lifecycle_system_hpp
#define dge_projectile_lifecycle_system_hpp

#include "../ecs/registry.hpp"
#include "../components/projectile_component.hpp"

namespace dge 
{

class projectile_lifecycle_system : public ecs::system
{
public:

    projectile_lifecycle_system()
    {
        require_component<ecs::projectile_component>();
    }

    void update()
    {
        for (auto entity : entities()) {
            auto projectile = entity.get_component<ecs::projectile_component>();
            if (SDL_GetTicks() - projectile.start_time > projectile.duration) {
                entity.destroy();
            }
        }
    }

};

}

#endif