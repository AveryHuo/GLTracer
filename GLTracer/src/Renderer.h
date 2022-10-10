#pragma once
#include "Scene.h"
#include "Sphere.h"
#include <map>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Renderer
{
protected:
	GLFWwindow* mainWindow;
	Scene* scene;
	Sphere* sphere;

	bool inputFirstMouse = true;
	float inputLastX, inputLastY, inputXPos, inputYPos, inputPitch = 1.0f;
	float inputYaw = -90.0f;
	float deltaTime = 0.0f; // Time between current frame and last frame
	float lastFrame = 0.0f; // Time of last frame

	bool enableMainCameraControl;
public:
	float materialValue1;
	~Renderer();
	Renderer(GLFWwindow* window, Scene* scene);
	void LoadScene();
	void Ready();
	void Update();
	void Draw();
	Scene* GetScene() const { return scene; };
	void SetEnableMainCameraControl(bool enable);
	void HandleCameraControl();

	void WindowSizeChange(GLFWwindow* window, int width, int height);
	void WindowMouseInputCallBack(GLFWwindow* window, double xpos, double ypos);
	void WindowMouseScrollInputCallBack(GLFWwindow* window, double xoffset, double yoffset);
};

