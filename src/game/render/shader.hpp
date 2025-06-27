//
// Created by andy on 6/25/2025.
//

#pragma once
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <glad/gl.h>

namespace game {

    enum class ShaderStageType {
        Vertex                = GL_VERTEX_SHADER,
        Fragment              = GL_FRAGMENT_SHADER,
        Geometry              = GL_GEOMETRY_SHADER,
        TesselationControl    = GL_TESS_CONTROL_SHADER,
        TesselationEvaluation = GL_TESS_EVALUATION_SHADER,
        Compute               = GL_COMPUTE_SHADER,
    };

    struct ShaderStage {
        std::string     source;
        ShaderStageType type;
    };

    class Shader {
    public:
        explicit Shader(const std::vector<ShaderStage>& stages);
        ~Shader();

        Shader(const Shader& other)                = delete;
        Shader(Shader&& other) noexcept            = default;
        Shader& operator=(const Shader& other)     = delete;
        Shader& operator=(Shader&& other) noexcept = default;

        static std::shared_ptr<Shader> loadSimpleShader(const std::filesystem::path& vertexPath,
                                                        const std::filesystem::path& fragmentPath);

        [[nodiscard]] int getUniformLocation(const char* name) const;

        [[nodiscard]] inline unsigned int getHandle() const { return m_Handle; };

        void use() const;

    private:
        unsigned int m_Handle;
    };

} // namespace game
