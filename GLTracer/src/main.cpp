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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

const char* glsl_version = "#version 130";
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
std::string resDir = "./res/";

Renderer* render;
Scene* scene;
Material * mat4;
bool show_demo_window = true;
bool show_another_window = false;
static int counter = 0;
ImVec4 directionColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
ImVec4 spotColor = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
float spotPos[3] = { 0, 2.0f, -2.0f };
float spotDir[3] = {0,-1,0};
float planeRot = -90.0f;
float spotAmbient = 0.05f;

float constant = 1.0f;
float linear = 0.0448f;
float quadratic = 0.032f;
float cutOff = 12.5f; //degrees
float outerCutOff = 17.5f;//degrees

void MainLoop(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	for (auto& quad : scene->GetQuads()) {
		quad->ChangeRot(planeRot, glm::vec3(1, 0, 0));
	}

	for (auto& light : scene->GetPointLights()) {
		light->SetDiffuse(glm::vec3(directionColor.x, directionColor.y, directionColor.z));
	}

	for (auto& light : scene->GetPointLights()) {
		light->SetDiffuse(glm::vec3(spotColor.x, spotColor.y, spotColor.z));
		light->ChangePos(glm::vec3(spotPos[0], spotPos[1], spotPos[2]));
		light->SetConstant(constant);
		light->SetLinear(linear);
		light->SetQuadratic(quadratic);
		light->SetAmbient(glm::vec3(spotAmbient));
	}

	render->Update(window);
	render->Draw();

	//swap buffer ºÍ´¦Àíevent
	glfwPollEvents();
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);
		
		ImGui::SliderFloat("quad rot", &planeRot, 0.0f, 360.0f);
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("diretional color", (float*)&directionColor); // Edit 3 floats representing a color
		ImGui::ColorEdit3("point color", (float*)&spotColor); // Edit 3 floats representing a color
		ImGui::SliderFloat3("point pos", (float*)&spotPos, -10, 10);
		ImGui::SliderFloat3("point direction", (float*)&spotDir, -1, 1);
		ImGui::SliderFloat("point constant", &constant, 0.0f, 1.0f);
		ImGui::SliderFloat("point linear", &linear, 0.0f, 1.0f);
		ImGui::SliderFloat("point quadratic", &quadratic, 0.0f, 1.0f);
		ImGui::SliderFloat("point Ambient", &spotAmbient, 0.0f, 1.0f);
		
		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			{
			counter++;
			scene->AddBox(glm::vec3(0.0f, 0.8f, -2.0f), mat4);
			scene->Load();
			}
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

void InitRender() {
	scene = new Scene();
	scene->SetSceneSize(SCR_WIDTH, SCR_HEIGHT);
	auto mat1 = scene->AddMaterial("vec", "frag");
	auto mat2 = scene->AddMaterial("vec_2", "frag_2");
	auto mat3 = scene->AddMaterial("vec_3_viewspace", "frag_3_viewspace");
	mat4 = scene->AddMaterial("vec_struct", "frag_struct");
	auto dir = scene->AddDirectionLight(glm::vec3(2.0f, 0.6f, -2.0f));
	dir->ChangeScale(glm::vec3(0.1f));
	dir->SetAmbient(glm::vec3(0.002f));
	dir->SetDiffuse(glm::vec3(1.0f, 0, 0));
	dir->SetDirection(glm::vec3(2.0f, 0.6f, -2.0f));

	auto point = scene->AddPointLight(glm::vec3(0.0f, 0.6f, -2.0f));
	point->ChangeScale(glm::vec3(0.1f));

	auto point2 = scene->AddPointLight(glm::vec3(0.0f, 1.0f, -2.0f));
	point2->ChangeScale(glm::vec3(0.1f));

	auto spot = scene->AddSpotLight(glm::vec3(0.0f, 2.0f, -2.0f));
	spot->ChangeScale(glm::vec3(0.1f));

	/*auto spot2 = scene->AddSpotLight(glm::vec3(-2.0f, 0, -2.0f));
	spot2->ChangeScale(glm::vec3(0.1f));
	spot2->SetDirection(glm::vec3(-2.0f, 0, -2.0f));*/
	

	scene->AddBox(glm::vec3(2.0f, -1.8f, -2.0f), mat4);
	scene->AddSphere(glm::vec3(-1.0f, -1.8f, -2.0f), mat4);
	scene->AddCylinder(glm::vec3(-2.0f, -1.8f, -2.0f), mat4);
	scene->AddQuad(glm::vec3(0.0f, -2.5f, -2.0f), mat4);
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
	scene->AddTexture(GL_TEXTURE0, resDir + std::string("block.jpg"));
	scene->AddTexture(GL_TEXTURE1, resDir + std::string("over.png"));
	scene->AddCamera(2.5f, true);

	render = new Renderer(scene);
	render->SetEnableMainCameraControl(true);
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

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glEnable(GL_DEPTH_TEST);

	InitRender();

	if (render == nullptr) {
		return 0;
	}

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
		{
			// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
			ImGuiIO& io = ImGui::GetIO();
			io.AddMousePosEvent((float)x, (float)y);

			// (2) ONLY forward mouse data to your underlying app/game.
			if (!io.WantCaptureMouse) {
				render->WindowMouseInputCallBack(window, x, y);
			}
		});
	glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.AddMouseWheelEvent((float)xoffset, (float)yoffset);

			// (2) ONLY forward mouse data to your underlying app/game.
			if (!io.WantCaptureMouse) {
				render->WindowMouseScrollInputCallBack(window, xoffset, yoffset);
			}
		});
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int x, int y)
		{
			render->WindowSizeChange(window, x, y);
		});

	glfwSetWindowUserPointer(window, render);
	while (!glfwWindowShouldClose(window)) {
		MainLoop(window);
	}
	delete render;
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}