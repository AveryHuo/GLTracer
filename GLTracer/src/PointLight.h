#pragma once
#include "BaseModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float POINT_LIGHT_PHYSICS_VERTEXS[] = {
                    -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                   -0.5f,  0.5f, -0.5f,
                   -0.5f, -0.5f, -0.5f,

                   -0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                   -0.5f,  0.5f,  0.5f,
                   -0.5f, -0.5f,  0.5f,

                   -0.5f,  0.5f,  0.5f,
                   -0.5f,  0.5f, -0.5f,
                   -0.5f, -0.5f, -0.5f,
                   -0.5f, -0.5f, -0.5f,
                   -0.5f, -0.5f,  0.5f,
                   -0.5f,  0.5f,  0.5f,

                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,

                   -0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f, -0.5f,
                    0.5f, -0.5f,  0.5f,
                    0.5f, -0.5f,  0.5f,
                   -0.5f, -0.5f,  0.5f,
                   -0.5f, -0.5f, -0.5f,

                   -0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f, -0.5f,
                    0.5f,  0.5f,  0.5f,
                    0.5f,  0.5f,  0.5f,
                   -0.5f,  0.5f,  0.5f,
                   -0.5f,  0.5f, -0.5f,
};

class PointLight:public BaseModel
{
protected:
    GLuint lightVbo;
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
public:
    void SetAmbient(glm::vec3 v) { ambient = v; };
    glm::vec3 GetAmbient() { return ambient; };
    void SetDiffuse(glm::vec3 v) { diffuse = v; };
    glm::vec3 GetDiffuse() { return diffuse; };
    void SetSpecular(glm::vec3 v) { specular = v; };
    glm::vec3 GetSpecular() { return specular; };
    void SetConstant(float v) { constant = v; };
    float GetConstant() { return constant; };
    void SetLinear(float v) { linear = v; };
    float GetLinear() { return linear; };
    void SetQuadratic(float v) { quadratic = v; };
    float GetQuadratic() { return quadratic; };

    void unInit() override {
        if (!isInit)
            return;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &lightVbo);
        isInit = false;
    }

    void init() override {
        if(isInit)
            return;

        if (bindMaterial != nullptr)
            bindMaterial->SetVector3("lightColor", diffuse);
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &lightVbo);
        glBindVertexArray(vao);
        //Insert all vertex data to vbo
        glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(POINT_LIGHT_PHYSICS_VERTEXS), POINT_LIGHT_PHYSICS_VERTEXS, GL_STATIC_DRAW);
        //Config vertex attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
        isInit = true;
    }
    void draw() const override {
        if (!isInit)
            return;
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void applyToMaterial(Material* mat, int idx) {
        mat->SetVector3(string_format("pointLights[%d].position", idx), this->GetPos());
        mat->SetVector3(string_format("pointLights[%d].ambient", idx), this->GetAmbient());
        mat->SetVector3(string_format("pointLights[%d].diffuse", idx), this->GetDiffuse());
        mat->SetVector3(string_format("pointLights[%d].specular", idx), this->GetSpecular());
        mat->SetFloat(string_format("pointLights[%d].constant", idx), this->GetConstant());
        mat->SetFloat(string_format("pointLights[%d].linear", idx), this->GetLinear());
        mat->SetFloat(string_format("pointLights[%d].quadratic", idx), this->GetQuadratic());
    }
};