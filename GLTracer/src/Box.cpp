#include "Box.h"


Box::Box():pos(glm::vec3(0, 0, 0)), rotBase(glm::vec3(0, 0, 0)), rotAngle(0)
{
}

Box::~Box()
{
}

Box::Box(glm::vec3 pos, float rotAngle, glm::vec3 rotBase):pos(pos), rotAngle(rotAngle), rotBase(rotBase)
{
}

void Box::ChangePos(glm::vec3 pos) {
    this->pos = pos;
}

void Box::ChangeRot(float rotAngle, glm::vec3 rotBase) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(rotAngle), rotBase);
}
