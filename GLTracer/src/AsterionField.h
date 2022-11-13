#pragma once
#include "BaseModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

class AsterionField:public BaseModel
{
protected:
    glm::mat4* modelMatrices;
    glm::mat4 projection;
    glm::mat4 view;
    Model *rockModel = nullptr;
    Material* rockMaterial = nullptr;
    Model *planetModel = nullptr;
    Material* planetMaterial = nullptr;
    unsigned int amount = 1000;
public:
    AsterionField(const std::string& rockPath, const std::string& planetPath) {
        rockModel = new Model(rockPath);
        rockMaterial = new Material();
        rockMaterial->CreateInstance();
        rockMaterial->AttachShaderByName("vec_asteroid_rock", GL_VERTEX_SHADER);
        rockMaterial->AttachShaderByName("frag_asteroid_rock", GL_FRAGMENT_SHADER);
        rockModel->SetMaterial(rockMaterial);


        planetModel = new Model(planetPath);
        planetMaterial = new Material();
        planetMaterial->CreateInstance();
        planetMaterial->AttachShaderByName("vec_asteroid_planet", GL_VERTEX_SHADER);
        planetMaterial->AttachShaderByName("frag_asteroid_planet", GL_FRAGMENT_SHADER);
        planetModel->SetMaterial(planetMaterial);
    }
    ~AsterionField() {
        if(rockModel)
            delete rockModel;
        if(planetModel)
            delete planetModel;
        if (rockMaterial)
            delete rockMaterial;
        if (planetMaterial)
            delete planetMaterial;
    }
    void unInit() override {
        if(!isInit)
            return;
        isInit = false;
    }

void init() override{
    if(isInit)
        return;

   
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime()); // initialize random seed	
    float radius = 50.0;
    float offset = 2.5f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z - 10.0f));

        // 2. scale: scale between 0.05 and 0.25f
        float scale = (rand() % 20) / 100.0f + 0.05;
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = (rand() % 360);
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }

    // configure instanced array
// -------------------------
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    // set transformation matrices as an instance vertex attribute (with divisor 1)
 // note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
 // normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
 // -----------------------------------------------------------------------------------------------------------------------------------
    auto meshes = rockModel->GetMeshes();
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        unsigned int VAO = meshes[i].GetVao();
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    isInit = true;
}

void SetProjection(glm::mat4 projMat) {
    projection = projMat;
}

void SetView(glm::mat4 viewMat) {
    view = viewMat;
}

void draw() override {
    if (!isInit)
        return;

    // configure transformation matrices
    rockMaterial->Use();
    rockMaterial->SetMatrix4("projection", 1, GL_FALSE, projection);
    rockMaterial->SetMatrix4("view", 1, GL_FALSE, view);

    // draw meteorites
    rockMaterial->Use();
    rockMaterial->SetInt("texture_diffuse1", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, rockModel->GetLoadedTextures()[0]->GetObject()); // note: we also made the textures_loaded vector public (instead of private) from the model class.
    auto meshes = rockModel->GetMeshes();
    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        glBindVertexArray(meshes[i].GetVao());
        glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
        glBindVertexArray(0);
    }
    rockMaterial->StopUsing();

    // draw planet
    planetMaterial->Use();
    planetMaterial->SetMatrix4("projection", 1, GL_FALSE, projection);
    planetMaterial->SetMatrix4("view", 1, GL_FALSE, view);

    planetModel->ChangePos(glm::vec3(0.0f, -3.0f, -10.0f));
    planetModel->ChangeScale(glm::vec3(1.0f, 1.0f, 1.0f));
    planetMaterial->SetMatrix4("model", 1, GL_FALSE, planetModel->GetTransform());
    planetModel->draw();
    planetMaterial->StopUsing();
}
};

