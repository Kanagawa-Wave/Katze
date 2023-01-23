#include "pch.h"

#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& path) {
    CreateShader(ReadFile(path));
}

Shader::~Shader() {
    glDeleteProgram(m_shader);
}

void Shader::Bind() const {
    glUseProgram(m_shader);
}

void Shader::UnBind() const {
    glUseProgram(0);
}

/* const std::string& name needs to match exactly with uniform name in shader source code! */
void Shader::UploadUniformInt(const std::string& name, const int& value) const
{
    glUseProgram(m_shader);
    glUniform1i(glGetUniformLocation(m_shader, name.c_str()), value);
}

void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
{
    glUseProgram(m_shader);
    glUniformMatrix4fv(glGetUniformLocation(m_shader, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
{
    glUseProgram(m_shader);
    glUniformMatrix3fv(glGetUniformLocation(m_shader, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) const
{
    glUseProgram(m_shader);
    glUniform4f(glGetUniformLocation(m_shader, name.c_str()), values.x, values.y, values.z, values.w);
}

void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) const
{
    glUseProgram(m_shader);
    glUniform3f(glGetUniformLocation(m_shader, name.c_str()), values.x, values.y, values.z);
}

void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) const
{
    glUseProgram(m_shader);
    glUniform2f(glGetUniformLocation(m_shader, name.c_str()), values.x, values.y);
}

void Shader::UploadUniformFloat(const std::string& name, const float& value) const
{
    glUseProgram(m_shader);
    glUniform1f(glGetUniformLocation(m_shader, name.c_str()), value);
}

void Shader::UploadUniformBuffer(const std::string& name, UniformBuffer& ubo) const
{
    glUseProgram(m_shader);
    uint32_t index = glGetUniformBlockIndex(m_shader, name.c_str());
    glUniformBlockBinding(m_shader, index, 0);
    ubo.Bind();
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GlobalUbo), &ubo.GetData());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

std::unordered_map<GLenum, std::vector<char>> Shader::ReadFile(const std::string& path) {
    std::ifstream fileVert(path + ".vert.spv", std::ios::ate | std::ios::binary);
    if (!fileVert.is_open())
        throw std::runtime_error("Failed to open file!");
    long long fileSize = fileVert.tellg();
    if (fileSize == 0)
        throw std::runtime_error(path + " doesn't exist!");
    std::vector<char> bufferVert(fileSize);
    fileVert.seekg(0);
    fileVert.read(bufferVert.data(), fileSize);
    fileVert.close();

    std::ifstream fileFrag(path + ".frag.spv", std::ios::ate | std::ios::binary);
    if (!fileFrag.is_open())
        throw std::runtime_error("Failed to open file!");
    fileSize = fileFrag.tellg();
    if (fileSize == 0)
        throw std::runtime_error(path + " doesn't exist!");
    std::vector<char> bufferFrag(fileSize);
    fileFrag.seekg(0);
    fileFrag.read(bufferFrag.data(), fileSize);
    fileFrag.close();

    std::unordered_map<GLenum, std::vector<char>> result;
    result[GL_VERTEX_SHADER] = bufferVert;
    result[GL_FRAGMENT_SHADER] = bufferFrag;
    return result;
}

void Shader::CreateShader(const std::unordered_map<GLenum, std::vector<char>>& input) {
    GLuint program = glCreateProgram();
    std::vector<GLenum> glShaderIDs(input.size());

    int index = 0;
    for (auto& kv : input) {
        const GLenum& type = kv.first;
        const std::vector<char>& spv = kv.second;

        GLuint s_id = glCreateShader(type);
        glShaderBinary(1, &s_id, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, spv.data(), (GLsizei) spv.size());
        glSpecializeShader(s_id, "main", 0, nullptr, nullptr);

        int compiled = 0;
        glGetShaderiv(s_id, GL_COMPILE_STATUS, &compiled);

        if (compiled)
            glAttachShader(program, s_id);
        glShaderIDs[index] = s_id;
        index++;
    }


    m_shader = program;
    glLinkProgram(program);

    int linked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

        // We don't need the program anymore.
        glDeleteProgram(program);

        for (auto id : glShaderIDs)
            glDeleteShader(id);

        LOG_ERROR("{0}", infoLog.data());
        LOG_ERROR("Shader link failure!");
        return;
    }


    for (auto s_id : glShaderIDs)
        glDetachShader(program, s_id);


}
