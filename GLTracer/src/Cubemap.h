#pragma once
#include <string>
#include <glad/glad.h>
class Cubemap
{
private:
	bool bLoaded;
	GLuint object;
	std::string type;
	std::string path; // store path of texture to compare with other textures
public:
	~Cubemap();
	Cubemap(const std::string& directory);
	std::string GetType() { return type; };
	std::string GetPath() { return path; };
	bool IsLoadSuccess();
	GLuint GetObject();
	void AddToPipeline(GLint textureChannel);
};

