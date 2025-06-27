//
// Created by andy on 6/25/2025.
//

#include "buffer.hpp"

namespace game {
    Buffer::Buffer(const std::size_t size, const void* data, const BufferUsage usage) {
        glCreateBuffers(1, &m_Handle);
        glNamedBufferData(m_Handle, size, data, static_cast<GLenum>(usage));
    }

    Buffer::~Buffer() { glDeleteBuffers(1, &m_Handle); }

    void Buffer::bind(BufferTarget target) const { glBindBuffer(static_cast<GLenum>(target), m_Handle); }

    void Buffer::data(const void* data, const std::size_t size, const BufferUsage usage) const {
        glNamedBufferData(m_Handle, size, data, static_cast<GLenum>(usage));
    }

    void Buffer::subdata(const void* data, const std::size_t size, const std::size_t offset) const {
        glNamedBufferSubData(m_Handle, offset, size, data);
    }
} // namespace game
