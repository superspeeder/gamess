//
// Created by andy on 6/25/2025.
//

#pragma once
#include <vector>

#include "glad/gl.h"

namespace game {

    enum class BufferUsage {
        StaticDraw = GL_STATIC_DRAW,
        StaticCopy = GL_STATIC_COPY,
        StaticRead = GL_STATIC_READ,

        DynamicDraw = GL_DYNAMIC_DRAW,
        DynamicCopy = GL_DYNAMIC_COPY,
        DynamicRead = GL_DYNAMIC_READ,

        StreamDraw = GL_STREAM_DRAW,
        StreamCopy = GL_STREAM_COPY,
        StreamRead = GL_STREAM_READ,
    };

    enum class BufferTarget {
        Array = GL_ARRAY_BUFFER,
        ElementArray = GL_ELEMENT_ARRAY_BUFFER,
        Uniform = GL_UNIFORM_BUFFER,
        ShaderStorage = GL_SHADER_STORAGE_BUFFER,
    };

    class Buffer {
    public:
        template<typename T>
        Buffer(const std::vector<T>& data, const BufferUsage usage) {
            glCreateBuffers(1, &m_Handle);
            glNamedBufferData(m_Handle, data.size() * sizeof(T), data.data(), static_cast<GLenum>(usage));
        }

        Buffer(std::size_t size, const void* data, BufferUsage usage);

        ~Buffer();

        Buffer(const Buffer& other)                = delete;
        Buffer(Buffer&& other) noexcept            = default;
        Buffer& operator=(const Buffer& other)     = delete;
        Buffer& operator=(Buffer&& other) noexcept = default;

        void bind(BufferTarget target) const;

        void data(const void* data, std::size_t size, BufferUsage usage) const;

        template<typename T>
        void data(const std::vector<T>& data, const BufferUsage usage) const {
            this->data(data.data(), data.size() * sizeof(T), usage);
        }

        void subdata(const void* data, std::size_t size, std::size_t offset) const;

        template<typename T>
        void subdata(const std::vector<T>& data, const std::size_t offset) const {
            subdata(data.data(), data.size() * sizeof(T), offset);
        }

        inline unsigned int getHandle() const { return m_Handle; }

    private:
        unsigned int m_Handle;
    };

} // namespace game
