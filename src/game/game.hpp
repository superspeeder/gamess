//
// Created by andy on 6/25/2025.
//

#pragma once
#include <glad/gl.h>
#include <memory>

#include <GLFW/glfw3.h>

#include "game/render/buffer.hpp"
#include "game/render/shader.hpp"
#include "game/render/texture.hpp"
#include "game/render/vertex_array.hpp"
#include "game/world/world.hpp"

namespace game {

    class Game {
    public:
        Game(const Game& other)                = delete;
        Game(Game&& other) noexcept            = delete;
        Game& operator=(const Game& other)     = delete;
        Game& operator=(Game&& other) noexcept = delete;

        Game();
        ~Game();

        void run();
        void frame();

    private:
        GLFWwindow* m_Window;

        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Buffer> m_VertexBuffer;
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<Texture> m_Texture;

        std::shared_ptr<World> m_World;

    };

} // namespace game
