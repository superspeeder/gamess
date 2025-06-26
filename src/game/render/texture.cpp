//
// Created by andy on 6/25/2025.
//

#include "texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>


namespace game {
    Texture::Texture(const int width, const int height, const GLenum format, const int internal_format,
                     const GLenum pixel_type, const void* data) {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
        glBindTexture(GL_TEXTURE_2D, m_Handle);
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, pixel_type, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    Texture::~Texture() { glDeleteTextures(1, &m_Handle); }

    void Texture::bind(const int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_Handle);
    }

    std::shared_ptr<Texture> Texture::load(const std::filesystem::path& path) {
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        const auto path_ = path.string();
        unsigned char* pixels = stbi_load(path_.c_str(), &width, &height, &channels, STBI_rgb_alpha);
        auto texture = std::make_shared<Texture>(width, height, GL_RGBA, GL_RGBA8, GL_UNSIGNED_BYTE, pixels);
        stbi_image_free(pixels);
        return texture;
    }

} // namespace game
