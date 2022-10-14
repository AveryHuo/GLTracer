#include "Texture.h"
#include "stb_image.h"


Texture::Texture(const std::string path, std::string type, bool gammaCorrection):path(path), type(type)
{
    bLoaded = false;
    //texture1
    glGenTextures(1, &object);

    //second parameter specifies the mipmap level
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);

    float* gammaData = nullptr;
    stbi_uc* linearData = nullptr;
    if (gammaCorrection) {
        gammaData = stbi_loadf(path.c_str(), &width, &height, &nrChannels, 0);
    }
    else {
        linearData = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    }
    if ((gammaData == nullptr) && (linearData == nullptr)) {
        printf("Failed to load texture\n");
        return;
    }

    GLenum format;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, object);
    if (gammaData)
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, gammaData); 
    else if(linearData)
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, linearData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR
    //Can not use Mipmap filter as Magnification filter! 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    bLoaded = true;

    if (gammaData)
        stbi_image_free(gammaData);
    else if (linearData)
        stbi_image_free(linearData);
}

bool Texture::IsLoadSuccess()
{
    return bLoaded;
}

GLuint Texture::GetObject()
{
	return object;
}

void Texture::AddToPipeline(GLint textureChannel)
{
    glActiveTexture(textureChannel);
    glBindTexture(GL_TEXTURE_2D, object);
}

Texture::~Texture() {
    printf("Texture deconstruct");
}

