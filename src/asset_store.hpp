#ifndef dge_asset_store_hpp
#define dge_asset_store_hpp

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

namespace dge
{

class asset_store
{
public:

    asset_store() = default;
    ~asset_store() { clear(); }

    void clear();

    void add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& path);
    SDL_Texture* get_texture(const std::string& asset_id) const;

    void add_font(const std::string& asset_id, const std::string& path, int font_size);
    TTF_Font* get_font(const std::string& asset_id);

private:

    using texture_map = std::map<std::string, SDL_Texture*>;
    using font_map = std::map<std::string, TTF_Font*>;

    texture_map m_textures;
    font_map m_fonts;

};

} // dge

#endif