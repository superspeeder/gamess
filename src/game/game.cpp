//
// Created by andy on 6/25/2025.
//

#include "game.hpp"

#include <glm/glm.hpp>
#include <iostream>

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
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        int mx, my;
        glfwGetMonitorPos(monitor, &mx, &my);

        glfwWindowHint(GLFW_POSITION_X, mx);
        glfwWindowHint(GLFW_POSITION_Y, my);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        m_Window = glfwCreateWindow(mode->width, mode->height, "Game", nullptr, nullptr);
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
        using clock = std::chrono::high_resolution_clock;


        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();


            int w, h;
            glfwGetFramebufferSize(m_Window, &w, &h);
            glViewport(0, 0, w, h);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            auto render_start = clock::now();
            m_World->render();
            auto render_end = clock::now();
            auto render_time = render_end - render_start;
            std::cout << render_time << '\n';

            glfwSwapBuffers(m_Window);
        }
    }
} // namespace game
