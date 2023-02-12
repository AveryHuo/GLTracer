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
//#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Scene.h"
#include "Renderer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "Model.h"
#include "LogConsole.h"

const char *glsl_version = "#version 130";
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
std::string resDir = "./res/";

Renderer *render;
Scene *scene;
Material *commonMat;
bool show_demo_window = false;
bool showLightEditor = true;
static int counter = 0;

// Objects
float planeRot = -90.0f;
float boxPosZ = -10.0f;

// Direction light
float dlPos[3] = {0.0f, 1.0f, -10};
ImVec4 dlDiffuse = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
float dlDirection[3] = { 0.0f, 0.0f, 0.0f };
float dlAmbient = 0.02f;
float dlSpecular[3] = {1.0f, 1.0f, 1.0f};

// Spot Light
float slPos[3] = {-0.2f, 0.6f, 0};
glm::vec3 slDirection = glm::vec3(0, 0, -1);
float slAmbient = 0.05f;
ImVec4 slDiffuse = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
glm::vec3 slSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
float slConstant = 1.0f;
float slLinear = 0.0448f;
float slQuadratic = 0.032f;
float slCutOff = 12.5f;		 // degrees
float slOuterCutOff = 17.5f; // degrees

// Point Light
float plPos[3] = {0.0f, 0.0f, 0};
float plAmbient = 0.05f;
ImVec4 plDiffuse = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
glm::vec3 plSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
float plConstant = 1.0f;
float plLinear = 0.09f;
float plQuadratic = 0.032f;

// Box testing
bool boxTesting = false;
std::vector<Box *>testBox;

// Mix testing
bool mixMaterialTesting = false;
Model * mixModel = nullptr;

float GetRandom(float A, float B) {
	return A + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (B - A)));
}

void MainLoop(GLFWwindow *window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	for (auto &quad : scene->GetQuads())
	{
		quad->ChangeRot(planeRot, glm::vec3(1, 0, 0));
	}

	for (auto& box : scene->GetBoxs()) {
		auto boxPos = box->GetPos();
		box->ChangePos(glm::vec3(boxPos.x, boxPos.y, boxPosZ));
	}

	for (auto &light : scene->GetDirLights())
	{
		light->ChangePos(glm::vec3(dlPos[0], dlPos[1], dlPos[2]));
		light->SetDiffuse(glm::vec3(dlDiffuse.x, dlDiffuse.y, dlDiffuse.z));
		light->SetDirection(glm::vec3(dlDirection[0], dlDirection[1], dlDirection[2]));
		light->SetAmbient(glm::vec3(dlAmbient));
		light->SetSpecular(glm::vec3(dlSpecular[0], dlSpecular[1], dlSpecular[2]));
	}

	for (auto &light : scene->GetPointLights())
	{
		light->ChangePos(glm::vec3(plPos[0], plPos[1], plPos[2]));
		light->SetDiffuse(glm::vec3(plDiffuse.x, plDiffuse.y, plDiffuse.z));
		light->SetAmbient(glm::vec3(plAmbient));
		light->SetSpecular(glm::vec3(plSpecular[0], plSpecular[1], plSpecular[2]));
		light->SetConstant(plConstant);
		light->SetLinear(plLinear);
		light->SetQuadratic(plQuadratic);
	}

	for (auto &light : scene->GetSpotLights())
	{
		light->ChangePos(glm::vec3(slPos[0], slPos[1], slPos[2]));
		light->SetDirection(glm::vec3(slDirection[0], slDirection[1], slDirection[2]));
		light->SetDiffuse(glm::vec3(slDiffuse.x, slDiffuse.y, slDiffuse.z));
		light->SetAmbient(glm::vec3(slAmbient));
		light->SetSpecular(glm::vec3(slSpecular[0], slSpecular[1], slSpecular[2]));
		light->SetConstant(slConstant);
		light->SetLinear(slLinear);
		light->SetQuadratic(slQuadratic);
		light->SetCutOff(slCutOff);
		light->SetOuterCutOff(slOuterCutOff);
	}

	render->Update(window);
	render->Draw();

	glfwPollEvents();
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	LogConsole::Instance().Draw("GLTracer: Console", &show_demo_window);

	ImGuiIO &io = ImGui::GetIO();
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
	{
		ImGui::ShowDemoWindow(&show_demo_window);
	}

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		ImGui::Begin("GlTracer Control");
		ImGui::Checkbox("IMGUI Demo", &show_demo_window); // Edit bools storing our window open/close state
		ImGui::Checkbox("Light Editor", &showLightEditor);

		ImGui::SliderFloat("quad rot", &planeRot, 0.0f, 360.0f);
		ImGui::SliderFloat("box Z", &boxPosZ, -20.0f, 20.0f);

		if (ImGui::Button("Add box")) // Buttons return true when clicked (most widgets return true when edited/activated)
		{
			if (!boxTesting) {
				for (int i = 0; i < 5; i++) {
					float randomX = GetRandom(-1.5, 1.5);
					float randomY = GetRandom(-4.0, -3.0);
					auto boxObj = scene->AddBox(glm::vec3(randomX, randomY, -10.0f), commonMat);
					boxObj->ChangeScale(glm::vec3(0.4f));
					testBox.push_back(boxObj);
				}
				scene->Load();
			}
			else {

				for (auto it = testBox.begin(); it != testBox.end();)
				{
					scene->RemoveBox(*it);
				}
				testBox.clear();
			}
			boxTesting = !boxTesting;
		}

		if (ImGui::Button("Geometry Test")) {
			auto mat_geo = scene->AddMaterial("vec_geometrytest_1", "frag_geometrytest_1", "geo_geometrytest_1");
			scene->AddCustomModel(mat_geo);
			scene->Load();
		}

		if (ImGui::Button("Asteroid Test")){
			scene->AddAsterionField(resDir + std::string("Models/rock/rock.obj"), resDir + std::string("Models/planet/planet.obj"));
			scene->Load();
		}

		if (ImGui::Button("Mix Material Test")) {
			if (!mixMaterialTesting) {
				auto mat5 = scene->AddMaterial("vec_struct_stencil_border", "frag_struct_stencil_border");
				auto mat_geo2 = scene->AddMaterial("vec_geometrytest_2", "frag_geometrytest_2", "geo_geometrytest_2");
				mixModel = scene->AddModelWithThreeMat(resDir + std::string("Models/nanosuit/nanosuit.obj"), glm::vec3(0.0f, -2.5f, -10.0f), commonMat, mat5, mat_geo2);
				scene->Load();
			}
			else {
				scene->RemoveModel(mixModel);
			}
			mixMaterialTesting = !mixMaterialTesting;
		}

		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (showLightEditor)
	{
		ImGui::SetWindowSize(ImVec2(200,200));
		ImGui::Begin("Light Editor", &showLightEditor); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

		if (ImGui::TreeNode("Direction Light"))
		{
			ImGui::SliderFloat3("Position", (float*)&dlPos, -10, 10);
			ImGui::ColorEdit3("Color", (float *)&dlDiffuse);
			ImGui::SliderFloat3("Direction", (float *)&dlDirection, -10, 10); 
			ImGui::SliderFloat3("Specular", (float*)&dlSpecular, -1, 1);
			ImGui::SliderFloat("Ambient", &dlAmbient, 0, 1);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Point Light"))
		{
			ImGui::SliderFloat3("Position", (float*)&plPos, -10, 10);
			ImGui::ColorEdit3("Color", (float*)&plDiffuse);
			ImGui::SliderFloat3("Specular", (float*)&plSpecular, -1, 1);
			ImGui::SliderFloat("Ambient", &plAmbient, 0, 1);
			ImGui::SliderFloat("point constant", &plConstant, 0.0f, 1.0f);
			ImGui::SliderFloat("point linear", &plLinear, 0.0f, 1.0f);
			ImGui::SliderFloat("point quadratic", &plQuadratic, 0.0f, 1.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Spot Light"))
		{
			ImGui::SliderFloat3("Position", (float*)&slPos, -10, 10);
			ImGui::ColorEdit3("Color", (float*)&slDiffuse);
			ImGui::SliderFloat3("Direction", (float*)&slDirection, -10, 10);
			ImGui::SliderFloat3("Specular", (float*)&slSpecular, -1, 1);
			ImGui::SliderFloat("Ambient", &slAmbient, 0, 1);
			ImGui::SliderFloat("Constant", &slConstant, 0.0f, 1.0f);
			ImGui::SliderFloat("Linear", &slLinear, 0.0f, 1.0f);
			ImGui::SliderFloat("Quadratic", &slQuadratic, 0.0f, 1.0f);
			ImGui::SliderFloat("CutOff", &slCutOff, 0.0f, 180.0f);
			ImGui::SliderFloat("Outer CutOff", &slOuterCutOff, 0.0f, 180.0f);
			ImGui::TreePop();
		}
		if (ImGui::Button("Close"))
			showLightEditor = false;
		ImGui::End();
	}
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

void InitRender()
{
/*
 Use as following codes to create your material
	//auto mat1 = scene->AddMaterial("vec", "frag");
	//auto mat2 = scene->AddMaterial("vec_2", "frag_2");
	//auto mat3 = scene->AddMaterial("vec_3_viewspace", "frag_3_viewspace");
	//auto env_mat = scene->AddMaterial("vec_struct_envmap", "frag_struct_envmap");
	//env_mat->SetTexture("material.texture_diffuse1", GL_TEXTURE0, texture1);
	//env_mat->SetTexture("material.texture_specular1", GL_TEXTURE1, texture2);
	//env_mat->SetCubemap("material.envmap", GL_TEXTURE2, scene->GetCubemaps()[0]);
*/

	scene = new Scene();

	scene->SetSceneSize(SCR_WIDTH, SCR_HEIGHT);

	//Skybox
	scene->AddSkybox(resDir + std::string("Cubemaps/skybox1"));
	// Lights
	auto dir = scene->AddDirectionLight(glm::vec3(2.0f, 0.6f, -2.0f));
	dir->ChangeScale(glm::vec3(0.1f));
	/*auto point = scene->AddPointLight(glm::vec3(0.0f, 0.6f, -2.0f));
	point->ChangeScale(glm::vec3(0.1f));
	auto spot = scene->AddSpotLight(glm::vec3(0.0f, 2.0f, -2.0f));
	spot->ChangeScale(glm::vec3(0.1f));*/


	// Textures may used
	auto texture1 = scene->AddTexture(resDir + std::string("Images/block.jpg"));
	auto texture2 = scene->AddTexture(resDir + std::string("Images/over.png"));


	// All Materials
	commonMat = scene->AddMaterial("vec_struct", "frag_struct");
	commonMat->SetTexture("material.texture_diffuse1", GL_TEXTURE0, texture2);
	commonMat->SetTexture("material.texture_specular1", GL_TEXTURE1, texture2);

	scene->AddSphere(glm::vec3(1.0f, -1.8f, -10.0f), commonMat);
	scene->AddCylinder(glm::vec3(-0.5f, -1.8f, -10.0f), commonMat);
	scene->AddQuad(glm::vec3(0.0f, -4.0f, -10.0f), commonMat);
	
	scene->AddCamera(2.5f, true);

	if (render = new Renderer(scene))
	{
		render->SetEnableMainCameraControl(true);
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	auto window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGl", NULL, NULL);
	if (window == NULL)
	{
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
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

	ImGui::StyleColorsDark();
	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);*/

	/*
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);*/
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	InitRender();

	if (render == nullptr)
	{
		return 0;
	}

	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y)
							 {
			// (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
			ImGuiIO& io = ImGui::GetIO();
			io.AddMousePosEvent((float)x, (float)y);

			// (2) ONLY forward mouse data to your underlying app/game.
			if (!io.WantCaptureMouse) {
				render->WindowMouseInputCallBack(window, x, y);
			} });
	glfwSetScrollCallback(window, [](GLFWwindow *window, double xoffset, double yoffset)
						  {
			ImGuiIO& io = ImGui::GetIO();
			io.AddMouseWheelEvent((float)xoffset, (float)yoffset);

			// (2) ONLY forward mouse data to your underlying app/game.
			if (!io.WantCaptureMouse) {
				render->WindowMouseScrollInputCallBack(window, xoffset, yoffset);
			} });
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int x, int y)
								   { render->WindowSizeChange(window, x, y); });

	glfwSetWindowUserPointer(window, render);

	while (!glfwWindowShouldClose(window))
	{
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