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

void Material::AttachShaderByName(const std::string shaderName, GLenum shaderType) {
    AttachShaderByPath(DEFAULT_SHADER_DIR + shaderName + ".glsl", shaderType);
}

void Material::AttachShaderByPath(const std::string shaderPath, GLenum shaderType) {
    auto shader = GenerateShaderInstance(shaderPath, shaderType);
    glAttachShader(object, shader->getObject());
    glLinkProgram(object);

    CheckResult();
    delete shader;
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

void Material::ReadyTextures() const{
    for (const auto& [textChannel, texture] : textureMap) {
        texture->AddToPipeline(textChannel);
    }
}

void Material::ReadyCubemaps() const {
    for (const auto& [textChannel, cubemap] : cubemapMap) {
        cubemap->AddToPipeline(textChannel);
    }
}

void Material::SetTexture(const std::string key, GLint textureChannel, Texture * texture)
{
    this->Use();
    if (textureChannel == GL_TEXTURE0) {
        SetInt(key, 0);
    }
    else if (textureChannel == GL_TEXTURE1) {
        SetInt(key, 1);
    }
    else if (textureChannel == GL_TEXTURE2) {
        SetInt(key, 2);
    }
    else if (textureChannel == GL_TEXTURE3) {
        SetInt(key, 3);
    }
    textureMap.insert({ textureChannel, texture });
    this->StopUsing();
}

void Material::SetCubemap(const std::string key, GLint channel, Cubemap *cubemap)
{
    this->Use();
    if (channel == GL_TEXTURE0) {
        SetInt(key, 0);
    }
    else if (channel == GL_TEXTURE1) {
        SetInt(key, 1);
    }
    else if (channel == GL_TEXTURE2) {
        SetInt(key, 2);
    }
    else if (channel == GL_TEXTURE3) {
        SetInt(key, 3);
    }
    cubemapMap.insert({ channel, cubemap });
    this->StopUsing();
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

