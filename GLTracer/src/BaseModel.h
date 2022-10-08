#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BaseModel
{
protected:
    glm::vec3 pos;
    glm::vec3 rotBase;
    float rotAngle;
public:
    BaseModel() {
        this->pos = glm::vec3(0);
        this->rotBase = glm::vec3(0,0,1);
        this->rotAngle = 0;
    }
    BaseModel(glm::vec3 pos, float rotAngle, glm::vec3 rotBase):pos(pos), rotAngle(rotAngle), rotBase(rotBase){}
    const glm::vec3 GetPos() const { return pos; };
    void ChangePos(glm::vec3 pos){this->pos = pos;};
    void ChangeRot(float rotAngle, glm::vec3 rotBase){
        this->rotAngle = rotAngle;
        this->rotBase = rotBase;
    }
    glm::mat4 GetTransform() const{
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, pos);
        modelMat = glm::rotate(modelMat, glm::radians(rotAngle), rotBase);
        return modelMat;
    }
    virtual int GetVertexCount() const = 0;
};