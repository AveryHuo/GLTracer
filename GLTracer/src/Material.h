#pragma once

#include <vector>
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#define DEFAULT_SHADER_DIR "./src/shaders/"

class Material
{
private:
    GLuint object;
    Shader* GenerateShaderInstance(const std::string path, GLenum shaderType);
    void CheckResult() ;

public:
    void CreateInstance();
    void DestroyInstance();

    bool operator < (const Material& m)const {return object < m.object;};

    void AttachVertexShaderByName(const std::string shaderName);
    void AttachVertexShaderByPath(const std::string shaderPath);
    void AttachFragmentShaderByName(const std::string shaderName);
    void AttachFragmentShaderByPath(const std::string shaderPath);
    void Use() const;
    void StopUsing() const;
    GLuint GetObject() const;
    void SetTextureSampler(const std::string key, int val) const;
    void SetFloat(const std::string key, float val)  const;
    void SetInt(const std::string key, int val)  const;
    void SetMatrix4(const std::string key,int count, GLboolean transport, glm::mat4 val)  const;
    void SetVector3(const std::string key, glm::vec3 val) const;
    void SetVector3(const std::string key, GLfloat val0, GLfloat val1, GLfloat val2) const;
};

