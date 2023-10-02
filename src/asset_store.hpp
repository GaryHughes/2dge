#ifndef dge_asset_store_hpp
#define dge_asset_store_hpp

#include <map>
#include <string>
#include <SDL2/SDL.h>

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

private:

    using texture_map = std::map<std::string, SDL_Texture*>;

    texture_map m_textures;

};

} // dge

#endif