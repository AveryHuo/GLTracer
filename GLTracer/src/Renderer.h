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
	Scene* scene;
	GLuint uboMatrices = 0;
	bool inputFirstMouse = true;
	float inputLastX = 1.0f;
	float inputLastY = 1.0f; 
	float inputXPos = 1.0f; 
	float inputYPos = 1.0f;
	float inputPitch = 1.0f;
	float inputYaw = -90.0f;
	float deltaTime = 0.0f; // Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	bool enableMainCameraControl;
public:
	float materialValue1;
	~Renderer();
	Renderer(Scene* scene);
	void LoadScene();
	void Ready();
	void Update(GLFWwindow* window);
	void Draw();
	Scene* GetScene() const { return scene; };
	void SetEnableMainCameraControl(bool enable);
	void HandleCameraControl(GLFWwindow* window);

	void WindowSizeChange(GLFWwindow* window, int width, int height);
	void WindowMouseInputCallBack(GLFWwindow* window, double xpos, double ypos);
	void WindowMouseScrollInputCallBack(GLFWwindow* window, double xoffset, double yoffset);
private:
	void HandleCursorPosCallback(GLFWwindow* window, double x, double y) ;
	void HandleScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
};

