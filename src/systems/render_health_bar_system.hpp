#ifndef dge_render_health_bar_system_hpp
#define dge_render_health_bar_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../asset_store.hpp"
#include "../components/health_component.hpp"
#include "../components/transform_component.hpp"
#include "../components/sprite_component.hpp"
#include "../logger.hpp"

namespace dge 
{

class render_health_bar_system : public ecs::system
{
public:

    render_health_bar_system()
    {
        require_component<ecs::health_component>();
        require_component<ecs::transform_component>();
        require_component<ecs::sprite_component>();
    }

    void update(SDL_Renderer* renderer, asset_store& assets, SDL_Rect& camera)
    {
        for (auto entity : entities()) {
            const auto transform = entity.get_component<ecs::transform_component>();
            const auto sprite = entity.get_component<ecs::sprite_component>();
            const auto health = entity.get_component<ecs::health_component>();

            SDL_Color color = {255, 255, 255};

            if (health.health_percentage >= 0 && health.health_percentage < 40) {
                color = {255, 0, 0};
            }
            else if (health.health_percentage >= 40 && health.health_percentage < 80) {
                color = {255, 255, 0};
            }
            else if (health.health_percentage >= 80 && health.health_percentage <= 100) {
                color = {0, 255, 0};
            }

            int bar_width = 15;
            int bar_height = 3;
            double bar_pos_x = (transform.position.x + (sprite.width * transform.scale.x)) - camera.x;
            double bar_pos_y = transform.position.y - camera.y;

            SDL_Rect bar_rect {
                int(bar_pos_x),
                int(bar_pos_y),
                int(bar_width * (health.health_percentage / 100.0)),
                int(bar_height)
            };

            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_RenderFillRect(renderer, &bar_rect);

            std::string health_text = std::to_string(health.health_percentage);

            auto font = assets.get_font("pico8-font-5");

            if (!font) {
                logger::error("FAILED TO LOAD FONT");
                continue;
            }

            SDL_Surface* surface = TTF_RenderText_Blended(font, health_text.c_str(), color);
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);

            int label_width = 0;
            int label_height = 0;
            SDL_QueryTexture(texture, nullptr, nullptr, &label_width, &label_height);
            SDL_Rect text_rect = {
                int(bar_pos_x),
                int(bar_pos_y) + 5,
                label_width,
                label_height
            };
            SDL_RenderCopy(renderer, texture, nullptr, &text_rect);
            SDL_DestroyTexture(texture);
        }
    }
};

}

#endif