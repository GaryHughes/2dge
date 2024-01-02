#include "level_loader.hpp"
#include "logger.hpp"
#include "components/transform_component.hpp"
#include "components/rigid_body_component.hpp"
#include "components/sprite_component.hpp"
#include "components/animation_component.hpp"
#include "components/box_collider_component.hpp"
#include "components/keyboard_control_component.hpp"
#include "components/camera_follow_component.hpp"
#include "components/projectile_emitter_component.hpp"
#include "components/health_component.hpp"
#include "components/text_label_component.hpp"
#include "components/script_component.hpp"
#include "game.hpp"
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <sol/sol.hpp>

namespace dge
{

level_loader::level_loader()
{

}

level_loader::~level_loader()
{

}

void level_loader::load_level(sol::state& lua, ecs::registry& registry, asset_store& assets, SDL_Renderer* renderer, int level_number)
{
    std::string filename = "../../assets/scripts/level_" + std::to_string(level_number) + ".lua";
    
    sol::load_result script = lua.load_file(filename);
    
    if (!script.valid()) {
        sol::error err = script;
        logger::error("failed to load " + filename + ": " + err.what());
        return;
    }

    lua.script_file(filename);

    sol::optional<sol::table> level = lua["Level"];

    if (!level.has_value()) {
        logger::error("file " + filename + " doesn't contain a Level table");
        return;
    }
    
    //
    // assets = {
    //      [0] =
    //      { type = "texture", id = "bullet-texture", file = "../../assets/images/bullet.png" },
    //      { type = "font", id = "charriot-font-20", file = "../../assets/fonts/charriot.ttf", font_size = 20 },
    //
    sol::table level_assets = (*level)["assets"];
    for (int index = 0; ; ++index) {
        sol::optional<sol::table> asset = level_assets[index];
        if (!asset.has_value()) {
            break;
        }
        std::string type = (*asset)["type"];
        std::string id = (*asset)["id"];
        std::string file = (*asset)["file"];
        
        if (type == "texture") {
            assets.add_texture(renderer, id, file);
        }
        else if (type == "font") {
            int font_size = (*asset)["font_size"];
            assets.add_font(id, file, font_size);
        }
    }

    // tilemap = {
    //     map_file = "../../assets/tilemaps/jungle.map",
    //     texture_asset_id = "tilemap-texture",
    //     num_rows = 20,
    //     num_cols = 25,
    //     tile_size = 32,
    //     scale = 2.0
    // },
    sol::table tilemap = (*level)["tilemap"];
    std::string texture_asset_id = tilemap["texture_asset_id"];
    std::string map_file = tilemap["map_file"];
    int tile_size = tilemap["tile_size"];
    double tile_scale = tilemap["scale"];

    auto tilemap_texture = assets.get_texture(texture_asset_id);
    SDL_Point size;
    SDL_QueryTexture(tilemap_texture, NULL, NULL, &size.x, &size.y);
    
    std::ifstream file(map_file);
    if (!file) {
        logger::error("unable to open file " + map_file);
        return;
    }
    
    int column = 0;
    int row = 0;
    int tiles_per_row = size.x / tile_size;
    int tiles_per_column = size.y / tile_size;
    int total_tiles = tiles_per_row * tiles_per_column;

    logger::info(map_file + " contains " + std::to_string(total_tiles) +  " tiles");

    int max_row = 0;
    int max_column = 0;

    while (!file.eof()) {
        std::string line;
        if (!std::getline(file, line)) {
            break;
        }
        logger::info("read map line " + line);
        std::vector<std::string> tokens;
        boost::split(tokens, line, boost::is_any_of(","));
        logger::info("line contains " + std::to_string(tokens.size()) + " tokens");
        for (const auto& token : tokens) {
            if (token.size() != 2) {
                logger::error("unexpected token " + token + " does not have 2 characters");
                continue;
            }
            int x = std::atoi(token.substr(1, 1).c_str());
            int y = std::atoi(token.substr(0, 1).c_str());

            if (x >= tiles_per_row) {
                logger::error("column " + std::to_string(x) + " > tiles per row " + std::to_string(tiles_per_row));
                continue;
            }

            if (y >= tiles_per_column) {
                logger::error("row " + std::to_string(y) + " > tiles per column " + std::to_string(tiles_per_column));
                continue;
            }
        
            ecs::entity tile = registry.create_entity();
            tile.group("tiles");
            tile.add_component<ecs::transform_component>(glm::vec2(column * (tile_size * tile_scale), row * (tile_size * tile_scale)), glm::vec2(tile_scale, tile_scale), 0.0);
            tile.add_component<ecs::sprite_component>(texture_asset_id, tile_size, tile_size, 0, false, x * tile_size, y * tile_size);

            column += 1;
        }

        if (column > max_column) {
            max_column = column;
        }

        column = 0;
        row += 1;

        if (row > max_row) {
            max_row = row;
        }
    }

    // game::s_map_width = int((*level)["map_width"]);
    // game::s_map_height = int((*level)["map_height"]);

    game::s_map_width = max_column * tile_size * tile_scale;
    game::s_map_height = max_row * tile_size * tile_scale;
  
    //
    // entities = {
    //     [0] =
    //     {
    //         tag = "player",
    //         components = {
    //             transform = {
    //                 position = { x = 240, y = 100 },
    //                 scale = { x = 1.0, y = 1.0 },
    //                 rotation = { 0.0 }
    //             },
    //
    logger::info("loading entities");
    sol::table level_entities = (*level)["entities"];
    for (int index = 0; ; ++index) {
        
        sol::optional<sol::table> entity_definition = level_entities[index];
        if (!entity_definition.has_value()) {
            break;
        }

        logger::info("creating entity with index = " + std::to_string(index));
        
        auto entity = registry.create_entity();
        
        sol::optional<std::string> tag = (*entity_definition)["tag"];
        if (tag.has_value()) {
            entity.tag(*tag);
        }
        
        sol::optional<std::string> group = (*entity_definition)["group"];
        if (group.has_value()) {
            entity.group(*group);
        }
        
        sol::optional<sol::table> components = (*entity_definition)["components"];
        if (!components.has_value()) {
            continue;
        }

        logger::info("loading components");

        // transform = {
        //     position = { x = 240, y = 100 },
        //     scale = { x = 1.0, y = 1.0 },
        //     rotation = 0.0 
        // },
        sol::optional<sol::table> transform = (*components)["transform"];
        if (transform.has_value()) {
            logger::info("adding transform component");
            entity.add_component<ecs::transform_component>(
                glm::vec2(
                    (*transform)["position"]["x"],
                    (*transform)["position"]["y"]
                ),
                glm::vec2(
                    (*transform)["scale"]["x"].get_or(1.0),
                    (*transform)["scale"]["y"].get_or(1.0)
                ),
                (*transform)["rotation"].get_or(0.0)
            );
        }

        // rigidbody = {
        //     velocity = { x = 0.0, y = 0.0 }
        // },
        sol::optional<sol::table> rigid_body = (*components)["rigidbody"];
        if (rigid_body.has_value()) {
            logger::info("adding rigidbody component");
            entity.add_component<ecs::rigid_body_component>(
                glm::vec2(
                    (*rigid_body)["velocity"]["x"],
                    (*rigid_body)["velocity"]["y"]
                )
            );
        }

        // sprite = {
        //     texture_asset_id = "chopper-texture",
        //     width = 32,
        //     height = 32,
        //     z_index = 5
        // },
        sol::optional<sol::table> sprite = (*components)["sprite"];
        if (sprite.has_value()) {
            std::string asset_id = (*sprite)["texture_asset_id"];
            logger::info("adding sprite component to entity: " + std::to_string(entity.id()) + " asset: " + asset_id);
            entity.add_component<ecs::sprite_component>(
                (*sprite)["texture_asset_id"],
                (*sprite)["width"],
                (*sprite)["height"],
                (*sprite)["z_index"].get_or(1),
                (*sprite)["fixed"].get_or(false),
                (*sprite)["src_rect_x"].get_or(0),
                (*sprite)["src_rect_y"].get_or(0)
            );
        }

        // animation = {
        //     num_frames = 2,
        //     speed_rate = 10
        // },
        sol::optional<sol::table> animation = (*components)["animation"];
        if (animation.has_value()) {
            logger::info("adding animation component");
            entity.add_component<ecs::animation_component>(
                (*animation)["num_frames"].get_or(1),
                (*animation)["speed_rate"].get_or(1)
            );
        }
        
        // boxcollider = {
        //     offset = { x = 0, y = 5 },
        //     width = 32,
        //     height = 25
        // },
        sol::optional<sol::table> box_collider = (*components)["boxcollider"];
        if (box_collider.has_value()) {
            logger::info("adding boxcollider component");
            entity.add_component<ecs::box_collider_component>(
                (*box_collider)["width"],
                (*box_collider)["height"],
                glm::vec2(
                    (*box_collider)["offset"]["x"].get_or(0),
                    (*box_collider)["offset"]["y"].get_or(0)
                )
            );
        }
                
        // health = {
        //     health_percentage = 100
        // },
        sol::optional<sol::table> health = (*components)["health"];
        if (health.has_value()) {
            logger::info("adding health component");
            entity.add_component<ecs::health_component>(
                (*health)["health_percentage"].get_or(100)
            );
        }
                
        // projectile_emitter = {
        //     projectile_velocity = { x = 100, y = 100 },
        //     projectile_duration = 10000,
        //     repeat_frequency = 0,
        //     hit_percentage_damage = 10,
        //     friendly = true
        // },
        sol::optional<sol::table> projectile_emitter = (*components)["projectile_emitter"];
        if (projectile_emitter.has_value()) {
            logger::info("adding projectile_emitter component");
            entity.add_component<ecs::projectile_emitter_component>(
                glm::vec2(
                    (*projectile_emitter)["projectile_velocity"]["x"].get_or(0),
                    (*projectile_emitter)["projectile_velocity"]["y"].get_or(0)
                ),
                (*projectile_emitter)["repeat_frequency"].get_or(1) * 1000,
                (*projectile_emitter)["projectile_duration"].get_or(10) * 1000,
                (*projectile_emitter)["hit_percentage_damage"].get_or(10),
                (*projectile_emitter)["friendly"].get_or(false)
            );
        }
        
        // keyboard_controller = {
        //     up_velocity = { x = 0, y = -50 },
        //     right_velocity = { x = 50, y = 0 },
        //     down_velocity = { x = 0, y = 50 },
        //     left_velocity = { x = -50, y = 0 }
        // },
        sol::optional<sol::table> keyboard_controller = (*components)["keyboard_controller"];
        if (keyboard_controller.has_value()) {
            logger::info("adding keyboard_controller component");
            entity.add_component<ecs::keyboard_control_component>(
                glm::vec2(
                    (*keyboard_controller)["up_velocity"]["x"].get_or(0.0),
                    (*keyboard_controller)["up_velocity"]["y"].get_or(0.0)
                ),
                glm::vec2(
                    (*keyboard_controller)["right_velocity"]["x"].get_or(0),
                    (*keyboard_controller)["right_velocity"]["y"].get_or(0)
                ),
                glm::vec2(
                    (*keyboard_controller)["down_velocity"]["x"].get_or(0),
                    (*keyboard_controller)["down_velocity"]["y"].get_or(0)
                ),
                glm::vec2(
                    (*keyboard_controller)["left_velocity"]["x"].get_or(0),
                    (*keyboard_controller)["left_velocity"]["y"].get_or(0)
                )
            ); 
        }
        
        // camera_follow = {
        //     follow = true
        // }
        sol::optional<sol::table> camera_follow = (*components)["camera_follow"];
        if (camera_follow.has_value() && (*camera_follow)["follow"].get_or(false)) {
            logger::info("adding camera_follow component");
            entity.add_component<ecs::camera_follow_component>();
        } 

        // on_update_script = {
        //     [0] =
        //     function(entity, delta_time, ellapsed_time)
        sol::optional<sol::table> update_script = (*components)["on_update_script"];
        if (update_script.has_value()) {
            logger::info("adding script component");
            sol::function function = (*update_script)[0];
            entity.add_component<ecs::script_component>(function);
        }
    }
    
    // ecs::entity label = registry.create_entity();
    // SDL_Color green = {0, 255, 0};
    // label.add_component<ecs::text_label_component>(glm::vec2(game::s_window_width / 2 - 40, 10), "CHOPPER 1.0", "charriot-font", green);
}

}