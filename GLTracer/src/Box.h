#pragma once
#include "DataUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float BOX_VERTEXS[] = {
                    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                   -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                   -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                   -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                   -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                   -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                   -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                   -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

class Box
{
private:
glm::vec3 pos;
glm::vec3 rotBase;
float rotAngle;

public:
Box();
~Box();
Box(glm::vec3 pos, float rotAngle, glm::vec3 rotBase);
const glm::vec3 GetPos() {return pos;};
void ChangePos(glm::vec3 pos);
void ChangeRot(float rotAngle, glm::vec3 rotBase);
int GetVertexCount() {
    return 36;
}
};

