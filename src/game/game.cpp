//
// Created by andy on 6/25/2025.
//

#include "game.hpp"

#include <glm/glm.hpp>

namespace game {
    struct Vertex {
        glm::vec2 position;
        glm::vec2 uv;
    };

    Game::Game() {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_Window = glfwCreateWindow(640, 480, "Game", nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        gladLoadGL(glfwGetProcAddress);

        std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {0.f, 0.f}},
            {{0.5f, -0.5f}, {1.f, 0.f}},
            {{0.5f, 0.5f}, {1.f, 1.f}},
            {{-0.5f, 0.5f}, {0.f, 1.f}},
        };

        m_VertexBuffer = std::make_shared<Buffer>(vertices, BufferUsage::StaticDraw);
        m_VertexArray  = std::make_shared<VertexArray>();
        m_VertexArray->bindVertexBuffer(*m_VertexBuffer, {2, 2});
        m_Shader = Shader::loadSimpleShader("res/shaders/main.vert", "res/shaders/main.frag");
        m_Texture = Texture::load("res/textures/test.png");
        m_World = std::make_shared<World>();
    }

    Game::~Game() { }

    void Game::run() {
        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            auto chunks = m_World->chunksInView({0, 0}, {1920, 1080});

            int w, h;
            glfwGetFramebufferSize(m_Window, &w, &h);
            glViewport(0, 0, w, h);

            glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->use();
            m_VertexArray->bind();

            // m_Texture->bind(0);
            // glUniform1i(glGetUniformLocation(m_Shader->getHandle(), "uTexture"), 0);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            glfwSwapBuffers(m_Window);
        }
    }
} // namespace game
