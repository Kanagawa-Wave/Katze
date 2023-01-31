#pragma once
#include "Buffers.h"

typedef unsigned int GLenum;

#include <glm/glm.hpp>

class Shader {
public:
    Shader() = default;
    Shader(const std::string& path);
    ~Shader();

    void Bind() const;
    void UnBind() const;

    void UploadUniformInt(const std::string& name, const int& value) const;

    void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;
    void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;

    void UploadUniformFloat4(const std::string& name, const glm::vec4& values) const;
    void UploadUniformFloat3(const std::string& name, const glm::vec3& values) const;
    void UploadUniformFloat2(const std::string& name, const glm::vec2& values) const;
    void UploadUniformFloat(const std::string& name, const float& value) const;

private:
    std::unordered_map<GLenum, std::vector<char>> ReadFile(const std::string& path);
    void CreateShader(const std::unordered_map<GLenum, std::vector<char>>& input);

    uint32_t m_shader;
};


