#include "asset_store.hpp"
#include "logger.hpp"
#include <SDL2/SDL_image.h>

namespace dge
{

void asset_store::clear()
{
    for (auto [_, texture] : m_textures) {
        SDL_DestroyTexture(texture);
    }
    m_textures.clear();

    for (auto [_, font] : m_fonts) {
        TTF_CloseFont(font);
    }
    m_fonts.clear();
}

void asset_store::add_texture(SDL_Renderer* renderer, const std::string& asset_id, const std::string& path)
{
    auto surface = IMG_Load(path.c_str());

    if (surface == nullptr) {
        logger::error("failed to load surface from file " + path);
        return;
    }

    auto texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        logger::error("failed to create texture from surface for file " + path);
        return;
    }

    logger::info("caching texture with id = " + asset_id + " for file " + path);

    m_textures.emplace(asset_id, texture);
}

SDL_Texture* asset_store::get_texture(const std::string& asset_id) const
{
    auto iterator = m_textures.find(asset_id);

    if (iterator == m_textures.end()) {
        logger::error("failed to find asset with id = " + asset_id);
        return nullptr;
    }

    return iterator->second;
}

void asset_store::add_font(const std::string& asset_id, const std::string& path, int font_size)
{
    auto font = TTF_OpenFont(path.c_str(), font_size);
    if (!font) {
        logger::error("Failed to load font: " + path);
        return;
    }
    m_fonts.emplace(asset_id, font);
}

TTF_Font* asset_store::get_font(const std::string& asset_id)
{   
    return m_fonts[asset_id];
}


} // dge