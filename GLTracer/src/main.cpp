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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
std::string resDir = "./res/";

Renderer* render;


void MainLoop(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	render->Update();
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
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Scene* scene = new Scene();
	scene->SetSceneSize(SCR_WIDTH, SCR_HEIGHT);
	auto mat1 = scene->AddMaterial("vec", "frag");
	auto mat2 = scene->AddMaterial("vec_2", "frag_2");
	auto mat3 = scene->AddMaterial("vec_3_viewspace", "frag_3_viewspace");
	auto mat4 = scene->AddMaterial("vec_struct", "frag_struct");
	auto lightMat = scene->AddMaterial("light_vec", "light_frag");
	scene->AddLight(glm::vec3(0.0f, 0.6f, 0.0f), lightMat);
	scene->AddBox(glm::vec3(2.0f, -1.8f, -2.0f), mat4);
	scene->AddSphere(glm::vec3(-1.0f, -1.8f, -2.0f), mat4);
	scene->AddCylinder(glm::vec3(-2.0f, -1.8f, -2.0f), mat4);
	//scene->AddBox(glm::vec3(0.0f, 0.0f, 0.0f), *mat1);
	//scene->AddBox(glm::vec3(2.0f, 5.0f, -15.0f), *mat1);
	//scene->AddBox(glm::vec3(-1.5f, -2.2f, -2.5f), *mat1);
	//scene->AddBox(glm::vec3(-3.8f, -2.0f, -12.3f), *mat1);
	//scene->AddBox(glm::vec3(2.4f, -0.4f, -3.5f), *mat1);
	//scene->AddBox(glm::vec3(-1.7f, 3.0f, -7.5f), *mat1);
	//scene->AddBox(glm::vec3(1.3f, -2.0f, -2.5f), *mat1);
	//scene->AddBox(glm::vec3(1.5f, 2.0f, -2.5f), *mat1);
	//scene->AddBox(glm::vec3(1.5f, 0.2f, -1.5f), *mat1);
	//scene->AddBox(glm::vec3(-1.3f, 1.0f, -1.5f), *mat1);
	scene->AddTexture(GL_TEXTURE0, resDir + std::string("block.jpg"), GL_RGB);
	scene->AddTexture(GL_TEXTURE1, resDir + std::string("over.png"), GL_RGBA);
	scene->AddCamera(2.5f, true);

	render = new Renderer(window, scene);
	render->SetEnableMainCameraControl(true);

	while (!glfwWindowShouldClose(window)) {
		MainLoop(window);
	}
	delete render;
	glfwTerminate();
	return 0;
}