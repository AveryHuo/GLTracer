#pragma once

#include <string>
#include "ShaderIncludes.h"
#include <glad/glad.h>

class Shader
{
private:
    GLuint object;
public:
    Shader(const ShaderInclude::ShaderSource& sourceObj, GLenum shaderType);
    GLuint getObject() const;
    ~Shader();
};