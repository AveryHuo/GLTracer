#pragma once
#include <string>
#include <glad/glad.h>
class Texture
{
private:
	bool bLoaded;
	GLuint object;
	std::string type;
	std::string path; // store path of texture to compare with other textures
public:
	~Texture();
	Texture(const std::string path, std::string type = "texture_diffuse");
	Texture(const std::string path, const std::string& directory, std::string type):Texture(directory + '/' + std::string(path), type){}
	std::string GetType(){return type;};
	std::string GetPath() { return path; };
	bool IsLoadSuccess();
	GLuint GetObject();
	void AddToPipeline(GLint textureChannel);
};

