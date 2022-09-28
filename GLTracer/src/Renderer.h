#pragma once
#include "Scene.h"
#include <map>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
protected:
	GLuint vbo;
	GLuint vao;
	GLFWwindow* mainWindow;
	Scene* scene;
public:
	float materialValue1;
	~Renderer();
	Renderer(GLFWwindow* window, Scene* scene);
	void InitGPUDataBuffers();
	void Ready();
	void Draw();
	Scene* GetScene() const { return scene; };
};

