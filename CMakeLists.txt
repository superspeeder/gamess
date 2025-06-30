cmake_minimum_required(VERSION 3.31)
project(gamess)

set(CMAKE_CXX_STANDARD 23)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_library(gl gl/src/gl.c)
target_include_directories(gl PUBLIC gl/include/)

add_library(fastnoise INTERFACE)
target_include_directories(fastnoise INTERFACE libs/fastnoise)

add_executable(game src/main.cpp)

add_library(game_internal
        src/game/game.cpp
        src/game/game.hpp
        src/game/render/shader.cpp
        src/game/render/shader.hpp
        src/game/render/buffer.cpp
        src/game/render/buffer.hpp
        src/game/render/vertex_array.cpp
        src/game/render/vertex_array.hpp
        src/game/render/texture.cpp
        src/game/render/texture.hpp
        src/game/world/world.cpp
        src/game/world/world.hpp
        src/game/world/world_generator.cpp
        src/game/world/world_generator.hpp
        src/game/utils/lru_cache.hpp
        src/game/world/tile.hpp
        src/game/render/camera.cpp
        src/game/render/camera.hpp)
target_include_directories(game_internal PUBLIC src/ ${Stb_INCLUDE_DIR})
target_link_libraries(game_internal PUBLIC glfw glm::glm gl fastnoise spdlog::spdlog)
target_compile_definitions(game_internal PUBLIC GLM_ENABLE_EXPERIMENTAL GLFW_INCLUDE_NONE)
target_compile_features(game_internal PUBLIC cxx_std_23)

target_link_libraries(game PRIVATE game_internal)


add_subdirectory(tests)
