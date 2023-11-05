add_executable(game
    main.cpp
    game.cpp game.hpp
    logger.cpp logger.hpp
    ecs/entity.cpp ecs/entity.hpp
    ecs/component.cpp ecs/component.hpp
    ecs/system.cpp ecs/system.hpp
    ecs/registry.cpp ecs/registry.hpp
    ecs/pool.hpp
    components/transform_component.hpp
    components/rigid_body_component.hpp
    components/sprite_component.hpp
    components/animation_component.hpp
    systems/movement_system.hpp
    systems/render_system.hpp
    systems/animation_system.hpp
    asset_store.cpp asset_store.hpp
)

find_package(Lua 5.4 REQUIRED)
target_include_directories(game PRIVATE ${LUA_INCLUDE_DIR})
target_include_directories(game PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_link_libraries(game 
    SDL2::Main 
    SDL2::Image
    ${LUA_LIBRARIES}
)