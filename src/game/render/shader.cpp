//
// Created by andy on 6/25/2025.
//

#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <cstring>

namespace game {
    Shader::Shader(const std::vector<ShaderStage>& stages) : m_Handle(glCreateProgram()) {
        std::vector<unsigned int> shaders;
        for (const auto& [source, type] : stages) {
            const unsigned int shader = glCreateShader(static_cast<GLenum>(type));
            const char*        text   = source.c_str();
            glShaderSource(shader, 1, &text, nullptr);
            glCompileShader(shader);
            int success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (success != GL_TRUE) {
                int length;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                char* log = new char[length];
                std::memset(log, '\0', length);
                glGetShaderInfoLog(m_Handle, length, nullptr, log);
                std::cerr << "Failed to compile shader: " << log << std::endl;
                delete[] log;
                // throw std::runtime_error("Failed to compile shader.");
            }
            glAttachShader(m_Handle, shader);
            shaders.push_back(shader);
        }
        glLinkProgram(m_Handle);
        int success;
        glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            int length;
            glGetProgramiv(m_Handle, GL_INFO_LOG_LENGTH, &length);
            char* log = new char[length];
            std::memset(log, '\0', length);
            glGetProgramInfoLog(m_Handle, length, nullptr, log);
            std::cerr << "Failed to link program: " << log << std::endl;
            delete[] log;
        }

        for (const unsigned int shader : shaders) {
            glDeleteShader(shader);
        }


        int numActiveUniforms = 0;
        glGetProgramiv(m_Handle, GL_ACTIVE_UNIFORMS, &numActiveUniforms);
        std::cout << numActiveUniforms << " active uniforms" << std::endl;

        for (int i = 0 ; i < numActiveUniforms; i++) {
            char name[128] = {};
            glGetActiveUniformName(m_Handle, i, 128, nullptr, name);
            std::cout << "- " << name << ": " << i << std::endl;
        }
    }

    Shader::~Shader() { glDeleteProgram(m_Handle); }

    static std::string loadFile(const std::filesystem::path& path) {
        std::ifstream f(path, std::ios::ate | std::ios::in);
        if (!f.is_open()) {
            throw std::runtime_error("Failed to open file");
        }
        const auto    sz = static_cast<std::size_t>(f.tellg());
        f.seekg(0, std::ios::beg);
        std::string text(sz, '\0');
        f.read(text.data(), sz);
        f.close();
        return text;
    }

    std::shared_ptr<Shader> Shader::loadSimpleShader(const std::filesystem::path& vertexPath,
                                                     const std::filesystem::path& fragmentPath) {
        const std::string vtext = loadFile(vertexPath);
        const std::string ftext = loadFile(fragmentPath);
        return std::make_shared<Shader>(
            std::vector<ShaderStage>{{vtext, ShaderStageType::Vertex}, {ftext, ShaderStageType::Fragment}});
    }

    int Shader::getUniformLocation(const char* name) const {
        return glGetUniformLocation(m_Handle, name);
    }

    void Shader::use() const {
        glUseProgram(m_Handle);
    }
} // namespace game
