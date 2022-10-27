#include "Cubemap.h"
#include "stb_image.h"
#include <vector>

Cubemap::Cubemap(const std::string& directory)
{
    bLoaded = false;
    //texture1
    glGenTextures(1, &object);
    glBindTexture(GL_TEXTURE_CUBE_MAP, object);

    //second parameter specifies the mipmap level
    int width, height, nrChannels;
    std::vector<std::string> faces{
        directory + "/" + "right.jpg",
        directory + "/" + "left.jpg",
        directory + "/" + "top.jpg",
        directory + "/" + "bottom.jpg",
        directory + "/" + "front.jpg",
        directory + "/" + "back.jpg",
    };

    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            printf("Cubemap texture failed to load at path: %s \n", faces[i].c_str());
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    bLoaded = true;
}

bool Cubemap::IsLoadSuccess()
{
    return bLoaded;
}

GLuint Cubemap::GetObject()
{
    return object;
}

void Cubemap::AddToPipeline(GLint textureChannel)
{
    glActiveTexture(textureChannel);
    glBindTexture(GL_TEXTURE_CUBE_MAP, object);
}

Cubemap::~Cubemap() {
    printf("Cubemap deconstruct");
}

