#ifndef dge_render_text_system_hpp
#define dge_render_text_system_hpp

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "../ecs/registry.hpp"
#include "../asset_store.hpp"
#include "../components/text_label_component.hpp"
#include "../logger.hpp"

namespace dge 
{

class render_text_system : public ecs::system
{
public:

    render_text_system()
    {
        require_component<ecs::text_label_component>();
    }

    void update(SDL_Renderer* renderer, asset_store& assets, SDL_Rect& camera)
    {
        for (auto entity : entities()) {
            const auto text_label = entity.get_component<ecs::text_label_component>();
            auto font = assets.get_font(text_label.asset_id);
            auto* surface = TTF_RenderText_Blended(font, text_label.text.c_str(), text_label.color);
            auto* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            
            int label_width = 0;
            int label_height = 0;

            SDL_QueryTexture(texture, nullptr, nullptr, &label_width, &label_height);

            SDL_Rect dst_rect = {
                int(text_label.position.x - (text_label.is_fixed ? 0 : camera.x)),
                int(text_label.position.y - (text_label.is_fixed ? 0 : camera.y)),
                label_width,
                label_height
            };

            SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);
            SDL_DestroyTexture(texture);
        }
    }

};

}

#endif