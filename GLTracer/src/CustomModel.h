#pragma once
#include "BaseModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float CUSTOM_VERTEXS[] = {
         -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
};

class CustomModel:public BaseModel
{
protected:
    GLuint vbo;
public:
    void unInit() override {
        if(!isInit)
            return;
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        isInit = false;
    }

void init() override{
    if(isInit)
        return;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    //Insert all vertex data to vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CUSTOM_VERTEXS), CUSTOM_VERTEXS, GL_STATIC_DRAW);
    
    //Config vertex attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    isInit = true;
}
void draw() override {
    if (!isInit)
        return;

    if (bindMaterial)
        bindMaterial->ReadyTextures();
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, 4);
    glBindVertexArray(0);
}
};

