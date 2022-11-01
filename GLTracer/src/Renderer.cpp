#include "Renderer.h"
#include <stdio.h>
#include "imgui/imgui.h"
#include "MaterialHelper.h"


Renderer::Renderer(Scene* scene) :scene(scene), materialValue1(0.2f), enableMainCameraControl(false)
{
	if (scene == nullptr)
	{
		printf("No Scene Found\n");
		return;
	}
	LoadScene();
	Ready();
}


void Renderer::LoadScene() {
	scene->Load();
}

void Renderer::Ready() {

	//Bind Camera
	for (auto& mat : scene->GetAllMaterials()) {
		auto uniformBlockIndex = glGetUniformBlockIndex(mat->GetObject(), "Matrices");
		glUniformBlockBinding(mat->GetObject(), uniformBlockIndex, 0);
	}
	// create the buffer
	glGenBuffers(1, &uboMatrices);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	// define the range of the buffer that links to a uniform binding point
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
}


void Renderer::Update(GLFWwindow* window)
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (enableMainCameraControl) {
		HandleCameraControl(window);
	}
}

void Renderer::SetEnableMainCameraControl(bool enable) {
	enableMainCameraControl = enable;	
	
};

void Renderer::HandleCursorPosCallback(GLFWwindow* window, double x, double y) {
	Renderer* engine = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	engine->WindowMouseInputCallBack(window, x, y);
}

void Renderer::HandleScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Renderer* engine = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	engine->WindowMouseScrollInputCallBack(window, xoffset, yoffset);
}

void Renderer::HandleCameraControl(GLFWwindow* mainWindow) {
	if (glfwGetKey(mainWindow, GLFW_KEY_UP) == GLFW_PRESS)
	{
		float mixValue = materialValue1;
		mixValue += 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue >= 1.0f)
			mixValue = 1.0f;
		materialValue1 = mixValue;
	}
	if (glfwGetKey(mainWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		float mixValue = materialValue1;
		mixValue -= 0.001f; // change this value accordingly (might be too slow or too fast based on system hardware)
		if (mixValue <= 0.0f)
			mixValue = 0.0f;
		materialValue1 = mixValue;
	}

	if (scene == nullptr)
		return;
	if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraForward(deltaTime);
	if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraBack(deltaTime);
	if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraLeft(deltaTime);
	if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS)
		scene->GetMainCamera()->MoveCameraRight(deltaTime);
}


void Renderer::WindowSizeChange(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (scene == nullptr)
		return;
	scene->SetSceneSize((float)width, (float)height);
}


void Renderer::WindowMouseInputCallBack(GLFWwindow* window, double xpos, double ypos)
{
	if (scene == nullptr)
		return;

	if(!enableMainCameraControl)
		return;

	if (inputFirstMouse)
	{
		inputLastX = (float)xpos;
		inputLastY = (float)ypos;
		inputFirstMouse = false;
	}
	float xoffset = (float)xpos - inputLastX;
	float yoffset = inputLastY - (float)ypos;
	inputLastX = (float)xpos;
	inputLastY = (float)ypos;

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		inputYaw += xoffset;
		inputPitch += yoffset;
		if (inputPitch > 89.0f)
			inputPitch = 89.0f;
		if (inputPitch < -89.0f)
			inputPitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(inputYaw)) * cos(glm::radians(inputPitch));
		direction.y = sin(glm::radians(inputPitch));
		direction.z = sin(glm::radians(inputYaw)) * cos(glm::radians(inputPitch));
		scene->GetMainCamera()->SetCameraFront(glm::normalize(direction));
	}else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS) {
		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;
		auto pos = scene->GetMainCamera()->GetCameraPos();
		pos.x += xoffset;
		pos.y += yoffset;
		scene->GetMainCamera()->SetCameraPos(pos);
	}
}

void Renderer::WindowMouseScrollInputCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	if (scene == nullptr)
		return;

	if (!enableMainCameraControl)
		return;

	float zoom = scene->GetMainCamera()->GetCameraFov();
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
	scene->GetMainCamera()->SetCameraFov(zoom);
}

void Renderer::Draw()
{
	auto mainCamera = scene->GetMainCamera();
	if (mainCamera == nullptr) {
		printf("[ERROR] No Main Camera Found!\n");
		return;
	}

	//Render过程
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// Must clear depth buffer otherwise can not show anything！
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // also clear the depth buffer now!
	glStencilMask(0x00);
	//glStencilMask(0xFF);
	//glStencilMask(0x00);
	//glStencilFunc(GL_EQUAL, 1, 0x00);
	//glDepthMask(GL_FALSE);//关闭深度写入常常会用来渲染透明片元。 关闭时无法写入深度测试缓存区。
	//glDepthFunc(GL_LESS);

	glm::mat4 view = mainCamera->GetViewMatrix();
	glm::mat4 proj = mainCamera->GetProjectMatrix();

	//Only need to set once!
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(proj));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	for (auto& light : scene->GetDirLights()) {
		auto mat = light->GetMaterial();
		if (mat != nullptr) {
			mat->Use();
			mat->SetMatrix4("model", 1, GL_FALSE, light->GetTransform());
			light->draw();
			mat->StopUsing();
		}
	}
	for (auto& light : scene->GetPointLights()) {
		auto mat = light->GetMaterial();
		mat->Use();
		mat->SetMatrix4("model", 1, GL_FALSE, light->GetTransform());
		light->draw();
		mat->StopUsing();
	}
	for (auto& light : scene->GetSpotLights()) {
		auto mat = light->GetMaterial();
		mat->Use();
		mat->SetMatrix4("model", 1, GL_FALSE, light->GetTransform());
		light->draw();
		mat->StopUsing();
	}
	
	for (auto& box : scene->GetBoxs()) {
		auto mat = box->GetMaterial();
		mat->Use();
		mat->SetBool("useNormalMap", false);

		MaterialHelper::AddLightsToMaterial(scene, mat);

		mat->SetFloat("material.shininess", 32.0f);
		
		mat->SetVector3("viewPos", mainCamera->GetCameraPos());

		mat->SetFloat("mixValue", materialValue1);

		//box->ChangeScale(glm::vec3(1));
		box->ChangeRot((float)glfwGetTime() * 25.0f, glm::vec3(0, 1, 0));
		mat->SetMatrix4("model", 1, GL_FALSE, box->GetTransform());
		box->draw();
		mat->StopUsing();
	}

	for (auto& sphere : scene->GetSpheres()) {
		auto mat = sphere->GetMaterial();
		mat->Use();
		MaterialHelper::AddLightsToMaterial(scene, mat);
		mat->SetBool("useNormalMap", false);
		mat->SetFloat("material.shininess", 32.0f);

		mat->SetVector3("viewPos", mainCamera->GetCameraPos());

		mat->SetFloat("mixValue", materialValue1);

		sphere->ChangeScale(glm::vec3(1));
		sphere->ChangeRot((float)glfwGetTime() * 25.0f, glm::vec3(0, 1, 0));
		mat->SetMatrix4("model", 1, GL_FALSE, sphere->GetTransform());
		sphere->draw();
		mat->StopUsing();
	}

	for (auto& cylinder : scene->GetCylinders()) {
		auto mat = cylinder->GetMaterial();
		mat->Use();
		MaterialHelper::AddLightsToMaterial(scene, mat);
		mat->SetBool("useNormalMap", false);
		mat->SetFloat("material.shininess", 32.0f);

		mat->SetVector3("viewPos", mainCamera->GetCameraPos());

		mat->SetFloat("mixValue", materialValue1);

		cylinder->ChangeScale(glm::vec3(1));
		cylinder->ChangeRot((float)glfwGetTime() * 25.0f, glm::vec3(0, 1, 0));
		mat->SetMatrix4("model", 1, GL_FALSE, cylinder->GetTransform());
		cylinder->draw();
		mat->StopUsing();
	}

	for (auto& quad : scene->GetQuads()) {
		auto mat = quad->GetMaterial();
		mat->Use();
		MaterialHelper::AddLightsToMaterial(scene, mat);
		mat->SetBool("useNormalMap", false);
		mat->SetFloat("material.shininess", 32.0f);

		mat->SetVector3("viewPos", mainCamera->GetCameraPos());

		mat->SetFloat("mixValue", materialValue1);

		quad->ChangeScale(glm::vec3(8));
		//quad->ChangeRot(-90.0f, glm::vec3(1, 0, 0));
		mat->SetMatrix4("model", 1, GL_FALSE, quad->GetTransform());
		quad->draw();
		mat->StopUsing();
	}

	for (auto& model : scene->GetModels()) {
		//Set all stencil to 1, and draw only when 1&0xFF
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);//allow write
		model->ChangeScale(glm::vec3(0.2f));
		auto mat = model->GetMaterial();
		mat->Use();
		MaterialHelper::AddLightsToMaterial(scene, mat);
		mat->SetBool("useNormalMap", false);
		mat->SetFloat("material.shininess", 32.0f);
		mat->SetVector3("viewPos", mainCamera->GetCameraPos());
		mat->SetFloat("mixValue", materialValue1);
		mat->SetMatrix4("model", 1, GL_FALSE, model->GetTransform());
		model->draw();
		mat->StopUsing();
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);

		//Set all stencil to 0, and draw only not 1&0xFF
		model->ChangeScale(glm::vec3(0.2005f));
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);//disallow write to stencil
		auto mat2 = model->GetMaterial2();
		mat2->Use();
		MaterialHelper::AddLightsToMaterial(scene, mat2);
		mat2->SetBool("useNormalMap", false);
		mat2->SetFloat("material.shininess", 32.0f);
		mat2->SetVector3("viewPos", mainCamera->GetCameraPos());
		mat2->SetFloat("mixValue", materialValue1);
		mat2->SetMatrix4("model", 1, GL_FALSE, model->GetTransform());
		model->draw(*mat2);
		mat2->StopUsing();

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
	}

	for (auto& model : scene->GetCustomModels()) {
		auto mat = model->GetMaterial();
		mat->Use();
		model->draw();
		mat->StopUsing();
	}

	auto skybox = scene->GetSkybox();
	if (skybox != nullptr) {
		auto mat = skybox->GetMaterial();
		mat->Use();
		mat->SetMatrix4("projection", 1, GL_FALSE, proj);
		mat->SetMatrix4("view", 1, GL_FALSE, glm::mat4(glm::mat3(view)));
		skybox->draw();
		mat->StopUsing();
	}

}

Renderer::~Renderer()
{
	scene->Unload();
	delete scene;
}
