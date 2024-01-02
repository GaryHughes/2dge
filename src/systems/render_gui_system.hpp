#ifndef dge_render_gui_system_hpp
#define dge_render_gui_system_hpp

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/backends/imgui_impl_sdlrenderer2.h>
#include "../ecs/registry.hpp"
#include "../logger.hpp"
#include "../components/transform_component.hpp"
#include "../components/rigid_body_component.hpp"
#include "../components/sprite_component.hpp"
#include "../components/animation_component.hpp"
#include "../components/box_collider_component.hpp"
#include "../components/projectile_emitter_component.hpp"
#include "../components/health_component.hpp"

namespace dge 
{

class render_gui_system : public ecs::system
{
public:

    render_gui_system() = default;
  
    void update(ecs::registry& registry)
    {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();
        
        if (ImGui::Begin("Spawn Enemies")) {
            static int enemy_x_position = 0;
            static int enemy_y_position = 0;
            static int enemy_x_velocity = 0;
            static int enemy_y_velocity = 0;
            static int projectile_x_velocity = 100;
            static int projectile_y_velocity = 0;
            static int projectile_frequency = 5000;
            static int projectile_duration = 2000;
            static int projectile_hit_percentage = 10;
            ImGui::InputInt("enemy x position", &enemy_x_position);
            ImGui::InputInt("enemy y position", &enemy_y_position);
            ImGui::InputInt("enemy x velocity", &enemy_x_velocity);
            ImGui::InputInt("enemy y velocity", &enemy_y_velocity);
            ImGui::InputInt("projectile x velocity", &projectile_x_velocity);
            ImGui::InputInt("projectile y velocity", &projectile_y_velocity);
            ImGui::InputInt("projectile frequency", &projectile_frequency);
            ImGui::InputInt("projectile duration", &projectile_duration);
            ImGui::InputInt("projectile hit percentage", &projectile_hit_percentage);
            if (ImGui::Button("Spawn")) {
                auto tank = registry.create_entity();
                tank.group("enemies");
                const int tile_size = 32;
                tank.add_component<ecs::transform_component>(glm::vec2(enemy_x_position, enemy_y_position), glm::vec2(1.0, 1.0), 0.0);
                tank.add_component<ecs::rigid_body_component>(glm::vec2(enemy_x_velocity, enemy_y_velocity));
                tank.add_component<ecs::sprite_component>("tank-tiger-right-texture", tile_size, tile_size, 1);
                tank.add_component<ecs::box_collider_component>(tile_size, tile_size);
                tank.add_component<ecs::projectile_emitter_component>(glm::vec2(projectile_x_velocity, 
                                                                      projectile_y_velocity), 
                                                                      projectile_frequency, 
                                                                      projectile_duration, 
                                                                      projectile_hit_percentage, 
                                                                      false);
                tank.add_component<ecs::health_component>(100);
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    }

};

}

#endif