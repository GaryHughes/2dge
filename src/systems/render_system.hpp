#ifndef dge_render_system_hpp
#define dge_render_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"
#include "../logger.hpp"

namespace dge 
{

class render_system : public ecs::system
{
public:

    render_system()
    {
        require_component<ecs::transform_component>();
        require_component<ecs::sprite_component>();
    }

    void update(SDL_Renderer* renderer, const asset_store& assets, SDL_Rect& camera)
    {
        auto sorted_entities = entities();

        // TODO - cull here instead of in the loop below

        std::sort(sorted_entities.begin(), sorted_entities.end(), [&](ecs::entity& left, ecs::entity& right) {
            const auto& left_sprite = left.get_component<ecs::sprite_component>();
            const auto& right_sprite = right.get_component<ecs::sprite_component>();
            return left_sprite.z_index < right_sprite.z_index;
        });

        for (auto& entity: sorted_entities) {
            const auto& transform = entity.get_component<ecs::transform_component>();
            const auto& sprite = entity.get_component<ecs::sprite_component>();

            if (!sprite.is_fixed) {
                bool is_entity_outside_camera_view = 
                    transform.position.x + (transform.scale.x * sprite.width) < camera.x ||
                    transform.position.x > camera.x + camera.w ||
                    transform.position.y + (transform.scale.y * sprite.height) < camera.y ||
                    transform.position.y > camera.y + camera.h;

                if (is_entity_outside_camera_view) {
                    continue;
                }
            }

            SDL_Rect src_rect = sprite.src_rect; 

            SDL_Rect dst_rect = {
                int(transform.position.x - (sprite.is_fixed ? 0 : camera.x)),
                int(transform.position.y - (sprite.is_fixed ? 0 : camera.y)),
                int(sprite.width * transform.scale.x),
                int(sprite.height * transform.scale.y)
            };

            auto texture = assets.get_texture(sprite.asset_id);

            if (texture == nullptr) {
                logger::error("render_system failed to load asset with id = " + sprite.asset_id);
                continue;
            }

            // logger::info("rendering sprite " + sprite.asset_id + 
            //              " " + std::to_string(dst_rect.x) +
            //              " " + std::to_string(dst_rect.y) +
            //              " " + std::to_string(dst_rect.w) +
            //              " " + std::to_string(dst_rect.y) +
            //              " " + std::to_string(src_rect.x) +
            //              " " + std::to_string(src_rect.y) +
            //              " " + std::to_string(src_rect.w) + 
            //              " " + std::to_string(src_rect.h));

            SDL_RenderCopyEx(renderer, texture, &src_rect, &dst_rect, transform.rotation, nullptr, sprite.flip);
        }
    }

};

}

#endif