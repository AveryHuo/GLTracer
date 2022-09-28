#include "Texture.h"
#include "stb_image.h"

Texture::Texture(const std::string path, GLint colorRange, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
    bLoaded = false;
    //texture1
    glGenTextures(1, &object);
    glBindTexture(GL_TEXTURE_2D, object);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);//GL_LINEAR_MIPMAP_LINEAR
    //Can not use Mipmap filter as Magnification filter! 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

    //second parameter specifies the mipmap level
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, colorRange, width, height, 0, colorRange, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        bLoaded = true;
    }
    else {
        printf("Failed to load texture\n");
    }

    stbi_image_free(data);
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