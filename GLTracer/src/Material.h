#pragma once

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Cubemap.h"
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#define DEFAULT_SHADER_DIR "./src/shaders/"

class Material
{
private:
    GLuint object;
    std::map<int, Texture*> textureMap;
    std::map<int, Cubemap*> cubemapMap;
    Shader* GenerateShaderInstance(const std::string path, GLenum shaderType);
    void CheckResult() ;
    bool operator < (const Material& m)const { return object < m.object; };
public:
    void CreateInstance();
    void DestroyInstance();

    void AttachShaderByName(const std::string shaderName, GLenum shaderType);
    void AttachShaderByPath(const std::string shaderPath, GLenum shaderType);
    void Use() const;
    void StopUsing() const;
    void ReadyTextures() const;
    void ReadyCubemaps() const;
    GLuint GetObject() const;
    void SetTexture(const std::string key, GLint textureChannel, Texture* texture);
    void SetCubemap(const std::string key, GLint channel, Cubemap* cubemap);
    void SetBool(const std::string key, bool val)  const;
    void SetFloat(const std::string key, float val)  const;
    void SetInt(const std::string key, int val)  const;
    void SetMatrix4(const std::string key,int count, GLboolean transport, glm::mat4 val)  const;
    void SetVector3(const std::string key, glm::vec3 val) const;
    void SetVector3(const std::string key, GLfloat val0, GLfloat val1, GLfloat val2) const;
};

