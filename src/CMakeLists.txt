add_executable(main
    main.cpp
)

find_package(Lua 5.4 REQUIRED)
target_include_directories(main PRIVATE ${LUA_INCLUDE_DIR})
target_include_directories(main PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)

target_link_libraries(main 
    PRIVATE SDL2::SDL2
    ${LUA_LIBRARIES}
)