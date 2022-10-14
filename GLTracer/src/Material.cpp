#include <stdexcept>
#include "Material.h"
#include <glm/gtc/type_ptr.hpp>

void Material::CreateInstance()
{
    object = glCreateProgram();
}

void Material::DestroyInstance()
{
    glDeleteProgram(object);
}

void Material::CheckResult() {
    GLint success = 0;
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (success == GL_FALSE)
    {
        std::string msg("Error while linking vertex shader to program \n");
        GLint logSize = 0;
        glGetProgramiv(object, GL_INFO_LOG_LENGTH, &logSize);
        char* info = new char[logSize + 1];
        glGetShaderInfoLog(object, logSize, NULL, info);
        msg += info;
        delete[] info;
        glDeleteProgram(object);
        object = 0;
        printf("Error %s\n", msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
}

Shader* Material::GenerateShaderInstance(const std::string path, GLenum shaderType) {
    ShaderInclude::ShaderSource vecShaderSource = ShaderInclude::load(path);
    if (vecShaderSource.src.empty()) {
        return nullptr;
    }
    return new Shader(vecShaderSource, shaderType);
}

void Material::AttachVertexShaderByName(const std::string shaderName) {
    AttachVertexShaderByPath(DEFAULT_SHADER_DIR + shaderName + ".glsl");
}

void Material::AttachVertexShaderByPath(const std::string shaderPath) {
    auto vertShader = GenerateShaderInstance(shaderPath, GL_VERTEX_SHADER);
    glAttachShader(object, vertShader->getObject());
    glLinkProgram(object);

    CheckResult();
    delete vertShader;
}

void Material::AttachFragmentShaderByName(const std::string shaderName) {
    AttachFragmentShaderByPath(DEFAULT_SHADER_DIR + shaderName + ".glsl");
}
void Material::AttachFragmentShaderByPath(const std::string shaderPath) {
    auto fragShader = GenerateShaderInstance(shaderPath, GL_FRAGMENT_SHADER);
    glAttachShader(object, fragShader->getObject());
    glLinkProgram(object);

    CheckResult();
    delete fragShader;
}

void Material::Use() const
{
    glUseProgram(object);
}

void Material::StopUsing() const
{
    glUseProgram(0);
}

GLuint Material::GetObject() const
{
    return object;
}

void Material::SetTextureSampler(const std::string key, int val) const
{
    SetInt(key, val);
}

void Material::SetBool(const std::string key, bool val) const
{
    SetInt(key, val);
}

void Material::SetFloat(const std::string key, float val) const
{
    glUniform1f(glGetUniformLocation(object, key.c_str()), val);
}

void Material::SetInt(const std::string key, int val) const
{
    glUniform1i(glGetUniformLocation(object, key.c_str()), val);
}

void Material::SetMatrix4(const std::string key, int count, GLboolean transport, glm::mat4 val) const
{
    glUniformMatrix4fv(glGetUniformLocation(object, key.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void Material::SetVector3(const std::string key, glm::vec3 val) const
{
    glUniform3f(glGetUniformLocation(object, key.c_str()), val.r, val.g, val.b);
}

void Material::SetVector3(const std::string key,  GLfloat val0, GLfloat val1, GLfloat val2) const
{
    glUniform3f(glGetUniformLocation(object, key.c_str()), val0, val1, val2);
}

