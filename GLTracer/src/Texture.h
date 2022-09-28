#pragma once
#include <string>
#include <glad/glad.h>
class Texture
{
private:
	bool bLoaded;
	GLuint object;
public:
	~Texture();
	Texture(const std::string path, GLint colorRange) :Texture(path, colorRange, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR){}
	Texture(const std::string path, GLint colorRange, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	bool IsLoadSuccess();
	GLuint GetObject();
	void AddToPipeline(GLint textureChannel);
};

