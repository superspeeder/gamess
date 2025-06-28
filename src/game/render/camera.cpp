//
// Created by andy on 6/27/2025.
//

#include "camera.hpp"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace game {
    Camera::Camera(const glm::vec2 &size, const glm::vec2 &position)
        : m_ProjectionMatrix(glm::ortho(-size.x / 2.0f, size.x / 2.0f, -size.y / 2.0f, size.y / 2.0f)),
          m_Position(position), m_Size(size), m_Dirty(true) {
        update();
    }

    void Camera::update() {
        if (m_Dirty) {
            auto pos = glm::vec2(glm::floor(m_Position.x * 8.0f) / 8.0f, glm::floor(m_Position.y * 8.0f) / 8.0f);
            m_ViewMatrix        = glm::translate(glm::identity<glm::mat4>(), glm::vec3(-pos, 0.0f));
            m_InverseViewMatrix = glm::inverse(m_ViewMatrix);
            m_CombinedMatrix    = m_ProjectionMatrix * m_ViewMatrix;
            m_Dirty             = false;
        }
    }

    void Camera::move(const glm::vec2 &delta) {
        m_Position += delta;
        m_Dirty = true;
    }

    void Camera::setPosition(const glm::vec2 &position) {
        m_Position = position;
        m_Dirty    = true;
    }

    SmoothCamera::SmoothCamera(
        const glm::vec2 &size, const glm::vec2 &position, const SmoothCameraParameters &parameters
    )
        : m_Camera(size, position), m_Parameters(parameters), m_Velocity(0.0f), m_Acceleration(0.0f) {}

    void SmoothCamera::update(const float delta) {
        m_Velocity += m_Acceleration * delta;
        if (m_Acceleration == glm::vec2(0.0f)) {
            m_Velocity -= m_Parameters.damping * delta * m_Velocity;
        }

        const float speed = glm::length(m_Velocity);
        if (speed < m_Parameters.snapThreshold) {
            m_Velocity = glm::zero<glm::vec2>();
        }

        if (speed > m_Parameters.maxSpeed) {
            m_Velocity = glm::normalize(m_Velocity) * m_Parameters.maxSpeed; // fix velocity
        }

        if (speed > std::numeric_limits<float>::epsilon()) {
            move(m_Velocity * delta);
        }

        // float maxSpeed = m_Parameters.maxSpeed;
        // if (distance <= m_Parameters.dampingDistance) {
        //     const float closeness = std::fmin(1.0f, distance / m_Parameters.dampingDistance);
        //     maxSpeed              = m_Parameters.minSpeed +
        //         std::powf(closeness, m_Parameters.dampingFactor) * (maxSpeed - m_Parameters.minSpeed);
        //     std::cout << "damping\n";
        // }
        //
        // const float travelDistance = delta * maxSpeed;
        //
        // if (distance > m_Parameters.dragDistance && distance - m_Parameters.dragDistance > travelDistance) {
        //     maxSpeed = distance - m_Parameters.dragDistance; // drag time
        //     const auto unit = glm::normalize(v);
        //     m_Camera.move(unit * maxSpeed);
        //     std::cout << "dragging " << travelDistance << " " << distance << " " << m_Parameters.dragDistance << "\n";
        // } else {
        //     if (travelDistance >= distance) {
        //         m_Camera.setPosition(m_TargetPosition);
        //         std::cout << "tp\n";
        //     } else {
        //         const auto unit = glm::normalize(v);
        //         m_Camera.move(unit * maxSpeed * delta);
        //         std::cout << "unit\n";
        //     }
        // }
        // std::cout << "cam p (" << m_Camera.m_Position.x << ", " << m_Camera.m_Position.y << ")\n";
        // std::cout << "target p (" << m_TargetPosition.x << ", " << m_TargetPosition.y << ")\n";

        m_Camera.update();
    }
} // namespace game
