#include "Renderer.h"
#include <stdio.h>


Renderer::Renderer(GLFWwindow* window, Scene* scene) : mainWindow(window), scene(scene), materialValue1(0.2f), enableMainCameraControl(false)
{
	if (window == nullptr)
	{
		printf("No Window Found\n");
		return;
	}
	if (scene == nullptr)
	{
		printf("No Scene Found\n");
		return;
	}
	glfwSetWindowUserPointer(window, this);
	LoadScene();
	Ready();
}


void Renderer::LoadScene() {
	scene->Load();
}

void Renderer::Ready() {
	glfwSetFramebufferSizeCallback(mainWindow, [](GLFWwindow* window, int x, int y)
		{
			Renderer* engine = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
			engine->WindowSizeChange(window, x, y);
		});
}


void Renderer::Update()
{
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	if (enableMainCameraControl) {
		HandleCameraControl();
	}
}

void Renderer::SetEnableMainCameraControl(bool enable) {
	enableMainCameraControl = enable;	
	if (enable) {
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(mainWindow, [](GLFWwindow* window, double x, double y)
			{
				Renderer* engine = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
				engine->WindowMouseInputCallBack(window, x, y);
			});
		glfwSetScrollCallback(mainWindow, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				Renderer* engine = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
				engine->WindowMouseScrollInputCallBack(window, xoffset, yoffset);
			});
	}
};

void Renderer::HandleCameraControl() {
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
	scene->SetSceneSize(width, height);
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
	if (zoom > 90.0f)
		zoom = 90.0f;
	scene->GetMainCamera()->SetCameraFov(zoom);
}

void Renderer::Draw()
{
	auto mainCamera = scene->GetMainCamera();
	if (mainCamera == nullptr) {
		printf("[ERROR] No Main Camera Found!\n");
		return;
	}

	auto textures = scene->GetTextureMap();
	if (textures.size() == 0) {
		return;
	}

	//Render过程
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	// Must clear depth buffer otherwise can not show anything！
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

	for (const auto& [textChannel, texture] : textures) {
		texture->AddToPipeline(textChannel);
	}

	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0, 0, -3));
	// Fixed Y, move camera X and Z
	view = mainCamera->GetViewMatrix();
	glm::mat4 proj = mainCamera->GetProjectMatrix();

	glm::vec3 lightPos = glm::vec3(1); 
	glm::vec3 lightColor = glm::vec3(1);
	glm::vec3 ambientColor = glm::vec3(1);
	
	for (auto& light : scene->GetLights()) {
		auto mat = light.GetMaterial();
		mat->Use();

		mat->SetMatrix4("view", 1, GL_FALSE, view);
		mat->SetMatrix4("projection", 1, GL_FALSE, proj);
		
		lightPos = light.GetPos();
		lightPos.x = cos(glfwGetTime());
		lightPos.z = sin(glfwGetTime());
		light.ChangePos(lightPos);
		light.ChangeScale(glm::vec3(0.2f));
		lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
		lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
		lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));
		lightColor = lightColor * glm::vec3(0.5);
		ambientColor = lightColor * glm::vec3(0.2);
		mat->SetVector3("lightColor", lightColor);
		mat->SetMatrix4("model", 1, GL_FALSE, light.GetTransform());
		light.draw();
		mat->StopUsing();
	}
	
	for (auto& box : scene->GetBoxs()) {
		auto mat = box.GetMaterial();
		mat->Use();
		mat->SetVector3("light.ambient", ambientColor);
		mat->SetVector3("light.diffuse", lightColor);
		mat->SetVector3("light.specular", 1.0f,1.0f,1.0f);
		mat->SetVector3("light.position", lightPos);

		mat->SetVector3("material.ambient", 0.0f, 0.1f, 0.06f);
		mat->SetVector3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		mat->SetVector3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		mat->SetFloat("material.shininess", 132.0f);

		mat->SetVector3("viewPos", mainCamera->GetCameraPos());

		mat->SetFloat("mixValue", materialValue1);
		mat->SetMatrix4("view", 1, GL_FALSE, view);
		mat->SetMatrix4("projection", 1, GL_FALSE, proj);

		box.ChangeScale(glm::vec3(1));
		box.ChangeRot((float)glfwGetTime() * 25.0f, glm::vec3(0, 1, 0));
		mat->SetMatrix4("model", 1, GL_FALSE, box.GetTransform());
		box.draw();
		mat->StopUsing();
	}

	for (auto& sphere : scene->GetSpheres()) {
		auto mat = sphere.GetMaterial();
		mat->Use();
		mat->SetVector3("light.ambient", ambientColor);
		mat->SetVector3("light.diffuse", lightColor);
		mat->SetVector3("light.specular", 1.0f, 1.0f, 1.0f);
		mat->SetVector3("light.position", lightPos);

		mat->SetVector3("material.ambient", 0.0f, 0.1f, 0.06f);
		mat->SetVector3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		mat->SetVector3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		mat->SetFloat("material.shininess", 132.0f);

		mat->SetVector3("viewPos", mainCamera->GetCameraPos());

		mat->SetFloat("mixValue", materialValue1);
		mat->SetMatrix4("view", 1, GL_FALSE, view);
		mat->SetMatrix4("projection", 1, GL_FALSE, proj);

		sphere.ChangeScale(glm::vec3(1));
		sphere.ChangeRot((float)glfwGetTime() * 25.0f, glm::vec3(0, 1, 0));
		mat->SetMatrix4("model", 1, GL_FALSE, sphere.GetTransform());
		sphere.draw();
		mat->StopUsing();
	}

	for (auto& cylinder : scene->GetCylinders()) {
		auto mat = cylinder.GetMaterial();
		mat->Use();
		mat->SetVector3("light.ambient", ambientColor);
		mat->SetVector3("light.diffuse", lightColor);
		mat->SetVector3("light.specular", 1.0f, 1.0f, 1.0f);
		mat->SetVector3("light.position", lightPos);

		mat->SetVector3("material.ambient", 0.0f, 0.1f, 0.06f);
		mat->SetVector3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		mat->SetVector3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		mat->SetFloat("material.shininess", 132.0f);

		mat->SetVector3("viewPos", mainCamera->GetCameraPos());

		mat->SetFloat("mixValue", materialValue1);
		mat->SetMatrix4("view", 1, GL_FALSE, view);
		mat->SetMatrix4("projection", 1, GL_FALSE, proj);

		cylinder.ChangeScale(glm::vec3(1));
		cylinder.ChangeRot((float)glfwGetTime() * 25.0f, glm::vec3(0, 1, 0));
		mat->SetMatrix4("model", 1, GL_FALSE, cylinder.GetTransform());
		cylinder.draw();
		mat->StopUsing();
	}

	//swap buffer 和处理event
	glfwPollEvents();
	glfwSwapBuffers(mainWindow);
}

Renderer::~Renderer()
{
	scene->Unload();
	delete scene;
}
