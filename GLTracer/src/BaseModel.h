#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"

class BaseModel
{
protected:
    GLuint vao;
    glm::vec3 scale;
    glm::vec3 pos;
    glm::vec3 rotBase;
    float rotAngle;
    Material *bindMaterial;
public:
    BaseModel() {
        this->bindMaterial = nullptr;
        this->vao = 0;
        this->scale = glm::vec3(1);
        this->pos = glm::vec3(0);
        this->rotBase = glm::vec3(0,0,1);
        this->rotAngle = 0;
    }
    BaseModel(glm::vec3 pos, float rotAngle, glm::vec3 rotBase):vao(0), scale(glm::vec3(1)), pos(pos), rotAngle(rotAngle), rotBase(rotBase), bindMaterial(nullptr) {}
    const glm::vec3 GetPos() const { return pos; };
    const glm::vec3 GetScale() const { return scale; };
    void ChangeScale(glm::vec3 scale) { this->scale = scale; };
    void ChangePos(glm::vec3 pos){this->pos = pos;};
    void ChangeRot(float rotAngle, glm::vec3 rotBase){
        this->rotAngle = rotAngle;
        this->rotBase = rotBase;
    }
    glm::mat4 GetTransform() const{
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, pos);
        modelMat = glm::rotate(modelMat, glm::radians(rotAngle), rotBase);
        modelMat = glm::scale(modelMat, scale);
        return modelMat;
    }
    void SetMaterial(Material* mat) {
        bindMaterial = mat;
    }
    Material* GetMaterial() const {
        return bindMaterial;
    }
    virtual void init() = 0;
    virtual void unInit() = 0;
    virtual void draw() const = 0;
};