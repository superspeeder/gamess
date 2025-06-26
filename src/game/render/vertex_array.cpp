//
// Created by andy on 6/25/2025.
//

#include "vertex_array.hpp"

namespace game {
    VertexArray::VertexArray() {
        glCreateVertexArrays(1, &m_Handle);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_Handle);
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_Handle);
    }

    void VertexArray::bindVertexBuffer(const Buffer& buffer, const std::vector<unsigned int>& attributes) {
        bind();
        buffer.bind(BufferTarget::Array);
        int stride = 0;
        for (int i = 0; i < attributes.size(); i++) {
            glVertexAttribFormat(i + m_NextAttribute, attributes[i], GL_FLOAT, false, stride);
            glVertexAttribBinding(i + m_NextAttribute, m_NextBinding);
            glEnableVertexAttribArray(i + m_NextAttribute);
            stride += attributes[i] * sizeof(float);
        }

        glBindVertexBuffer(m_NextBinding, buffer.getHandle(), 0, stride);

        m_NextAttribute += attributes.size();
        m_NextBinding++;
    }
} // game
