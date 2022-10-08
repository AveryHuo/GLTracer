#pragma once
#include "BaseModel.h"
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

class Box:public BaseModel
{

public:
int GetVertexCount() const override{
    return 36;
}
};

