#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Material.h"
#include "DataUtils.h"

class BaseModel
{
protected:
    Material *bindMaterial;
    Material* bindMaterial2;
    Material* bindMaterial3;
    GLuint vao;
    glm::vec3 scale;
    glm::vec3 pos;
    glm::vec3 rotBase;
    float rotAngle;
    bool isInit;
   
public:
    BaseModel() {
        this->bindMaterial = nullptr;
        this->bindMaterial2 = nullptr;
        this->bindMaterial3 = nullptr;
        this->vao = 0;
        this->scale = glm::vec3(1);
        this->pos = glm::vec3(0);
        this->rotBase = glm::vec3(0,0,1);
        this->rotAngle = 0;
        this->isInit = false;
    };
    BaseModel(BaseModel& other) {
        this->bindMaterial = other.bindMaterial;
        this->bindMaterial2 = other.bindMaterial2;
        this->bindMaterial3 = other.bindMaterial3;
        this->vao = other.vao;
        this->scale = other.scale;
        this->pos = other.pos;
        this->rotBase = other.rotBase;
        this->rotAngle = other.rotAngle;
        this->isInit = false;
    }
    BaseModel& operator=(BaseModel& other) {
        this->bindMaterial = other.bindMaterial;
        this->bindMaterial2 = other.bindMaterial2;
        this->bindMaterial3 = other.bindMaterial3;
        this->vao = other.vao;
        this->scale = other.scale;
        this->pos = other.pos;
        this->rotBase = other.rotBase;
        this->rotAngle = other.rotAngle;
        this->isInit = false;
        return *this;
    }

    BaseModel(glm::vec3 pos, float rotAngle, glm::vec3 rotBase): isInit(false), vao(0), scale(glm::vec3(1)), pos(pos), rotAngle(rotAngle), rotBase(rotBase), bindMaterial(nullptr), bindMaterial2(nullptr), bindMaterial3(nullptr) {}
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
    void SetMaterial2(Material* mat) {
        bindMaterial2 = mat;
    }
    Material* GetMaterial2() const {
        return bindMaterial2;
    }
    void SetMaterial3(Material* mat) {
        bindMaterial3 = mat;
    }
    Material* GetMaterial3() const {
        return bindMaterial3;
    }
    virtual void init() = 0;
    virtual void unInit() = 0;
    virtual void draw() = 0;
};