//
// Created by andy on 6/25/2025.
//

#pragma once
#include "buffer.hpp"

namespace game {

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray& other)                = delete;
        VertexArray(VertexArray&& other) noexcept            = default;
        VertexArray& operator=(const VertexArray& other)     = delete;
        VertexArray& operator=(VertexArray&& other) noexcept = default;

        void bind() const;

        void bindVertexBuffer(const Buffer& buffer, const std::vector<unsigned int> &attributes);

        unsigned int nextAttribute();
        unsigned int nextBinding();

        [[nodiscard]] inline unsigned int getHandle() const noexcept { return m_Handle; }

    private:
        unsigned int m_Handle{};
        unsigned int m_NextAttribute = 0;
        unsigned int m_NextBinding = 0;
    };

} // namespace game
