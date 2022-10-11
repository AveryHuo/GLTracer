#include "BaseModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float SPOT_LIGHT_PHYSICS_VERTEXS[] = {
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

class SpotLight:public BaseModel
{
protected:
    GLuint lightVbo;
    glm::vec3 direction = glm::vec3(0, 0, -1);
    glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    float cutOff = 12.5f; //degrees
    float outerCutOff = 15.0f;//degrees
public:
    void SetDirection(glm::vec3 v) { direction = v; };
    glm::vec3 GetDirection(){return direction;};
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
    void SetCutOff(float v) { cutOff = v; };
    float GetCutOff() { return cutOff; };
    void SetOuterCutOff(float v) { outerCutOff = v; };
    float GetOuterCutOff() { return outerCutOff; };



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
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &lightVbo);
        glBindVertexArray(vao);
        //Insert all vertex data to vbo
        glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(SPOT_LIGHT_PHYSICS_VERTEXS), SPOT_LIGHT_PHYSICS_VERTEXS, GL_STATIC_DRAW);
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
};