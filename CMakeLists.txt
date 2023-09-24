cmake_minimum_required(VERSION 3.10.0)

project(2dge)

set(CMAKE_CXX_CLANG_TIDY
    clang-tidy;
    -warnings-as-errors=*;
    )

# Don't embed build tree paths
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "")

set(BOOST_VERSION_REQUIRED 1.81)

include_directories(
    "/usr/local/include"
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/thirdparty/catch2/include
    ${CMAKE_SOURCE_DIR}/thirdparty
)

link_directories(
    "/usr/local/lib"
    "/opt/homebrew/lib"
)

enable_testing()
include(CTest)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/thirdparty/catch2/contrib")
include(Catch)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/thirdparty/cmake/sdl2")

add_subdirectory(src)
