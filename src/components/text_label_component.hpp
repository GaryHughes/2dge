#ifndef dge_ecs_text_label_component_h
#define dge_ecs_text_label_component_h

#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace dge {
namespace ecs {

struct text_label_component
{
    glm::vec2 position;
    std::string text;
    std::string asset_id;
    SDL_Color color;
    bool is_fixed;

    text_label_component(const glm::vec2& p = glm::vec2(0), const std::string& t = "", const std::string& id = "", const SDL_Color& c = {0, 0, 0}, bool fixed = true)
    :   position(p),
        text(t),   
        asset_id(id),
        color(c),
        is_fixed(fixed)
    {
    }
};

}
}

#endif