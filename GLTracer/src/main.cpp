#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "ShaderIncludes.h"
#include "Shader.h"
#include "Material.h"
#include "Texture.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include "Renderer.h"


std::string shaderDir = "./src/shaders/";
std::string resDir = "./res/";

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool firstMouse = true;
float lastX, lastY, xpos, ypos, pitch= 1.0f;
float yaw = -90.0f;

Renderer* render;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if(render == nullptr)
		return;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		float mixValue = render->materialValue1;
		mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
		render->materialValue1 = mixValue;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		float mixValue = render->materialValue1;
		mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
		render->materialValue1 = mixValue;
	}

	auto scene = render->GetScene();
	if (scene == nullptr)
		return;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraForward(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraBack(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraLeft(deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraRight(deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (render == nullptr)
		return;
	auto scene = render->GetScene();
	if(scene == nullptr)
		return;

	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;
	lastX = (float)xpos;
	lastY = (float)ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	yaw += xoffset;
	pitch += yoffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	scene->GetMainCamera()->SetCameraFront(glm::normalize(direction));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (render == nullptr)
		return;
	auto scene = render->GetScene();
	if (scene == nullptr)
		return;


	float zoom = scene->GetMainCamera()->GetCameraFov();
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
	scene->GetMainCamera()->SetCameraFov(zoom);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MainLoop(GLFWwindow* window) {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	//´¦ÀíÊäÈë
	processInput(window);

	render->Draw();
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGl", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create glfw windows" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	Scene* scene = new Scene();
	scene->SetSceneSize(SCR_WIDTH, SCR_HEIGHT);
	auto mat1 = scene->AddMaterial("vec", "frag");
	auto mat2 = scene->AddMaterial("vec", "frag1");
	scene->AddBox(glm::vec3(0.0f, 0.0f, 0.0f), *mat1);
	scene->AddBox(glm::vec3(2.0f, 5.0f, -15.0f), *mat1);
	scene->AddBox(glm::vec3(-1.5f, -2.2f, -2.5f), *mat1);
	scene->AddBox(glm::vec3(-3.8f, -2.0f, -12.3f), *mat1);
	scene->AddBox(glm::vec3(2.4f, -0.4f, -3.5f), *mat1);
	scene->AddBox(glm::vec3(-1.7f, 3.0f, -7.5f), *mat1);
	scene->AddBox(glm::vec3(1.3f, -2.0f, -2.5f), *mat1);
	scene->AddBox(glm::vec3(1.5f, 2.0f, -2.5f), *mat2);
	scene->AddBox(glm::vec3(1.5f, 0.2f, -1.5f), *mat2);
	scene->AddBox(glm::vec3(-1.3f, 1.0f, -1.5f), *mat1);
	scene->AddTexture(GL_TEXTURE0, resDir + std::string("d.jpeg"), GL_RGB);
	scene->AddTexture(GL_TEXTURE1, resDir + std::string("over.png"), GL_RGBA);
	scene->AddCamera(2.5f, true);

	render = new Renderer(window, scene);

	while (!glfwWindowShouldClose(window)) {
		MainLoop(window);
	}
	delete render;
	glfwTerminate();
	return 0;
}