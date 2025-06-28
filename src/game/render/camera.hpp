//
// Created by andy on 6/27/2025.
//

#pragma once

#include <glm/glm.hpp>

namespace game {

    class Camera {
      public:
        Camera(const glm::vec2 &size, const glm::vec2 &position);

        [[nodiscard]] inline glm::mat4 projectionMatrix() const { return m_ProjectionMatrix; }

        [[nodiscard]] inline glm::mat4 combinedMatrix() const { return m_CombinedMatrix; }

        [[nodiscard]] inline bool dirty() const { return m_Dirty; }

        [[nodiscard]] inline glm::mat4 viewMatrix() const { return m_ViewMatrix; }

        [[nodiscard]] inline glm::mat4 inverseViewMatrix() const { return m_InverseViewMatrix; }

        [[nodiscard]] inline glm::vec2 position() const { return m_Position; }
        [[nodiscard]] inline glm::vec2 size() const { return m_Size; }

        [[nodiscard]] inline glm::vec2 minCorner() const { return m_Position - m_Size / 2.0f; }
        [[nodiscard]] inline glm::vec2 maxCorner() const { return m_Position + m_Size / 2.0f; }


        void move(const glm::vec2 &delta);
        void setPosition(const glm::vec2 &position);

        void update();

      private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_CombinedMatrix{};
        glm::mat4 m_ViewMatrix{};
        glm::mat4 m_InverseViewMatrix{};
        glm::vec2 m_Position;

        glm::vec2 m_Size;

        bool m_Dirty;
        friend class SmoothCamera;
    };

    struct SmoothCameraParameters {
        float maxSpeed = 36.0;
        float damping = 4.0;
        float dragDistance = 4.0f;
        float snapThreshold = 0.01f;
    };

    class SmoothCamera {
      public:
        SmoothCamera(const glm::vec2 &size, const glm::vec2 &position, const SmoothCameraParameters &parameters);

        [[nodiscard]] inline glm::mat4 projectionMatrix() const { return m_Camera.m_ProjectionMatrix; }

        [[nodiscard]] inline glm::mat4 combinedMatrix() const { return m_Camera.m_CombinedMatrix; }

        [[nodiscard]] inline bool dirty() const { return m_Camera.m_Dirty; }

        [[nodiscard]] inline glm::mat4 viewMatrix() const { return m_Camera.m_ViewMatrix; }

        [[nodiscard]] inline glm::mat4 inverseViewMatrix() const { return m_Camera.m_InverseViewMatrix; }

        [[nodiscard]] inline glm::vec2 position() const { return m_Camera.m_Position; }

        inline void move(const glm::vec2 &delta) { m_Camera.move(delta); }

        void setPosition(const glm::vec2 &position) { m_Camera.setPosition(position); }

        inline void setVelocity(const glm::vec2 &velocity) { m_Velocity = velocity; };
        inline void setAcceleration(const glm::vec2 &accel) { m_Acceleration = accel; };

        void update(float delta);

        inline const Camera& operator*() const { return m_Camera; }

      private:
        Camera                 m_Camera;
        SmoothCameraParameters m_Parameters;
        glm::vec2              m_Velocity;
        glm::vec2              m_Acceleration;
    };
} // namespace game
