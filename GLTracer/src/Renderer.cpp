#include "Renderer.h"
#include <stdio.h>


Renderer::Renderer(GLFWwindow* window, Scene* scene) : vao(0), vbo(0), mainWindow(window), scene(scene), materialValue1(0.2f), enableMainCameraControl(false)
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
	InitGPUDataBuffers();
	Ready();
}


void Renderer::InitGPUDataBuffers() {
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	//bind vao
	glBindVertexArray(vao);
	//Insert all vertex data to vbo
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BOX_VERTEXS), BOX_VERTEXS, GL_STATIC_DRAW);
	//Config vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
}

void Renderer::Ready() {
	glfwSetFramebufferSizeCallback(mainWindow, [](GLFWwindow* window, int x, int y)
		{
			Renderer* engine = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
			engine->WindowSizeChange(window, x, y);
		});

	auto mats = scene->GetAllMaterials();
	auto textures = scene->GetTextureMap();
	if (mats.size() == 0) {
		return;
	}
	if (textures.size() == 0) {
		return;
	}

	for (Material* mat : mats) {
		mat->Use();
		for (const auto& [textChannel, texture] : textures) {
			if (textChannel == GL_TEXTURE0) {
				mat->SetTextureSampler("texture1", 0);
			}
			else if (textChannel == GL_TEXTURE1) {
				mat->SetTextureSampler("texture2", 1);
			}
		}
	}
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

	auto boxMaps = scene->GetBoxMap();
	for (auto& [mat, boxes] : boxMaps) {
		mat.Use();
		mat.SetFloat("mixValue", materialValue1);

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0, 0, -3));

		// Fixed Y, move camera X and Z
		view = mainCamera->GetViewMatrix();

		glm::mat4 proj = mainCamera->GetProjectMatrix();

		mat.SetMatrix4("view", 1, GL_FALSE, view);
		mat.SetMatrix4("projection", 1, GL_FALSE, proj);

		int i = 0;
		glBindVertexArray(vao);
		for (auto box : boxes) {
			// calculate the model matrix for each object and pass it to shader before drawing
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, box.GetPos());
			float angle = 20.0f * i;
			if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
				angle = (float)glfwGetTime() * 25.0f;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 0, 0.5f));
			mat.SetMatrix4("model", 1, GL_FALSE, model);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			i++;
		}
		mat.StopUsing();
	}

	//swap buffer 和处理event
	glfwPollEvents();
	glfwSwapBuffers(mainWindow);
}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	delete scene;
}
