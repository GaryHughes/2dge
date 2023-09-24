add_executable(game
    main.cpp
    game.cpp game.hpp
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