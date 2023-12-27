#ifndef dge_camera_movement_system_hpp
#define dge_camera_movement_system_hpp

#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/camera_follow_component.hpp"
#include "../components/transform_component.hpp"

namespace dge 
{

class camera_movement_system : public ecs::system
{
public:

    camera_movement_system()
    {
        require_component<ecs::camera_follow_component>();
        require_component<ecs::transform_component>();
    }

    void update(SDL_Rect& camera)
    {
        for (auto& entity: entities()) {
            
            auto& transform = entity.get_component<ecs::transform_component>();
            
            if (transform.position.x < (camera.w / 2) < game::s_map_width) {
                camera.x = transform.position.x - (game::s_window_width / 2);
            }

            if (transform.position.y < (camera.h / 2) < game::s_map_height) {
                camera.y = transform.position.y - (game::s_window_height / 2);
            }

            camera.x = camera.x < 0 ? 0 : camera.x;
            camera.y = camera.y < 0 ? 0 : camera.y;
            camera.x = camera.x > camera.w ? camera.w : camera.x;
            camera.y = camera.y > camera.h ? camera.h :camera.y;
        }
    }
};

}

#endif