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

        void bind() const;

        void bindVertexBuffer(const Buffer& buffer, const std::vector<unsigned int> &attributes);

    private:
        unsigned int m_Handle{};
        unsigned int m_NextAttribute = 0;
        unsigned int m_NextBinding = 0;
    };

} // namespace game
