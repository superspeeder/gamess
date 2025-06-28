//
// Created by andy on 6/25/2025.
//

#include "game.hpp"

#include <chrono>
#include <glm/glm.hpp>
#include <iostream>

namespace game {
    struct Vertex {
        glm::vec2 position;
        glm::vec2 uv;
    };

    constexpr SmoothCameraParameters CAMERA_PARAMS{};

    constexpr float     CAMERA_SPEED  = 72.f;
    constexpr glm::vec2 VIEWPORT_SIZE = {1920.0f, 1080.0f};
    constexpr glm::vec2 TILE_DIM      = {8.0f, 8.0f};

    Game::Game() : m_Camera(VIEWPORT_SIZE / TILE_DIM, {0.0f, 0.0f}, CAMERA_PARAMS) {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_CONTEXT_DEBUG, GLFW_TRUE);

        GLFWmonitor       *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode    = glfwGetVideoMode(monitor);
        int                mx, my;
        glfwGetMonitorPos(monitor, &mx, &my);

        glfwWindowHint(GLFW_POSITION_X, mx);
        glfwWindowHint(GLFW_POSITION_Y, my);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        // glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        // glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        // glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        // glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        m_Window = glfwCreateWindow(mode->width, mode->height, "Game", nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        gladLoadGL(glfwGetProcAddress);

        glfwSwapInterval(0);

        std::vector<Vertex> vertices = {
            {{-1.0f, -1.0f}, {0.f, 0.f}},
            {{1.0f, -1.0f}, {1.f, 0.f}},
            {{1.0f, 1.0f}, {1.f, 1.f}},
            {{-1.0f, 1.0f}, {0.f, 1.f}},
        };

        m_VertexBuffer = std::make_shared<Buffer>(vertices, BufferUsage::StaticDraw);
        m_VertexArray  = std::make_shared<VertexArray>();
        m_VertexArray->bindVertexBuffer(*m_VertexBuffer, {2, 2});
        m_Shader   = Shader::loadSimpleShader("res/shaders/main.vert", "res/shaders/main.frag");
        m_Texture  = Texture::load("res/textures/bg.png");
        m_Texture2 = Texture::load("res/textures/bg_underground.png");
        m_World    = std::make_shared<World>();
    }

    Game::~Game() {}

    void Game::run() {
        using clock = std::chrono::high_resolution_clock;
        float delta = 1.0f / 60.0f;
        m_ThisFrame = glfwGetTime();

        while (!glfwWindowShouldClose(m_Window)) {
            glfwPollEvents();

            int w, h;
            glfwGetFramebufferSize(m_Window, &w, &h);
            glViewport(0, 0, w, h);

            // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            // glClear(GL_COLOR_BUFFER_BIT);

            m_Shader->use();
            if (m_Camera.position().y > -80.0f) {
                m_Texture->bind(0);
            } else {
                m_Texture2->bind(0);
            }
            m_VertexArray->bind();
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            frame(delta);

            glfwSwapBuffers(m_Window);

            m_LastFrame = m_ThisFrame;
            m_ThisFrame = glfwGetTime();
            delta       = m_ThisFrame - m_LastFrame;
        }
    }

    void Game::frame(const float delta) {
        glm::vec2 movement = {0.0f, 0.0f};
        if (glfwGetKey(m_Window, GLFW_KEY_A)) {
            movement += glm::vec2{-1.0f, 0.0f};
        }

        if (glfwGetKey(m_Window, GLFW_KEY_D)) {
            movement += glm::vec2{1.0f, 0.0f};
        }

        if (glfwGetKey(m_Window, GLFW_KEY_W)) {
            movement += glm::vec2{0.0f, 1.0f};
        }

        if (glfwGetKey(m_Window, GLFW_KEY_S)) {
            movement += glm::vec2{0.0f, -1.0f};
        }

        if (movement != glm::vec2(0.0f)) {
            movement = glm::normalize(movement);
        }
        m_Camera.setAcceleration(movement * CAMERA_SPEED);


        m_Camera.update(delta);

        m_World->render(*m_Camera);
    }
} // namespace game
