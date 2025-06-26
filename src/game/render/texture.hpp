//
// Created by andy on 6/25/2025.
//

#pragma once

#include <filesystem>
#include <glad/gl.h>
#include <memory>

namespace game {

    class Texture {
    public:
        Texture(int width, int height, GLenum format, int internal_format, GLenum pixel_type, const void* data);
        ~Texture();

        void bind(int unit) const;


        static std::shared_ptr<Texture> load(const std::filesystem::path& path);

    private:
        unsigned int m_Handle;
    };

} // namespace game
