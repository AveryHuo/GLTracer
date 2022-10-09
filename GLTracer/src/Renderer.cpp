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
	InitGPUDataBuffers();
	Ready();
}


void Renderer::InitGPUDataBuffers() {
	
	/*
	//CPU, not recommanded
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
	glNormalPointer(GL_FLOAT, 0, &normals[0]);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	
	
	
	// GPU

		glBindVertexArray(vaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[0]);


	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, bufferHandle[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);*/

	////light
	//glGenVertexArrays(1, &lightVao);
	//glGenBuffers(1, &lightVbo);
	//glBindVertexArray(lightVao);
	////Insert all vertex data to vbo
	//glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(LIGHT_PHYSICS_VERTEXS), LIGHT_PHYSICS_VERTEXS, GL_STATIC_DRAW);
	////Config vertex attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);

	//float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);



	//box
	glGenVertexArrays(1, &boxVao);
	glGenBuffers(1, &boxVbo);
	glBindVertexArray(boxVao);
	//Insert all vertex data to vbo
	glBindBuffer(GL_ARRAY_BUFFER, boxVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BOX_VERTEXS), BOX_VERTEXS, GL_STATIC_DRAW);
	//Config vertex attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//sphere
	sphere = new Sphere();
	glGenVertexArrays(1, &sphereVao);
	glGenBuffers(1, &sphereVbo);
	glGenBuffers(1, &sphereEbo);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(sphereVao);

	glBindBuffer(GL_ARRAY_BUFFER, sphereVbo);
	glBufferData(GL_ARRAY_BUFFER, sphere->getVertexCount(), sphere->getVertices(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere->getIndexCount(), sphere->getIndices(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	

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
	auto lightMap = scene->GetLightMap();
	/*glBindVertexArray(lightVao);
	glBindVertexArray(boxVao);*/
	glBindVertexArray(boxVao);
	for (auto& [mat, lights] : lightMap) {
		mat.Use();

		mat.SetMatrix4("view", 1, GL_FALSE, view);
		mat.SetMatrix4("projection", 1, GL_FALSE, proj);
		
		
		for (auto& light : lights) {
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
			mat.SetVector3("lightColor", lightColor);
			mat.SetMatrix4("model", 1, GL_FALSE, light.GetTransform());
			light.Draw();
		}
		mat.StopUsing();
	}

	auto boxMaps = scene->GetBoxMap();
	for (auto& [mat, boxes] : boxMaps) {
		mat.Use();
		mat.SetVector3("light.ambient", ambientColor);
		mat.SetVector3("light.diffuse", lightColor);
		mat.SetVector3("light.specular", 1.0f,1.0f,1.0f);
		mat.SetVector3("light.position", lightPos);

		mat.SetVector3("material.ambient", 0.0f, 0.1f, 0.06f);
		mat.SetVector3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		mat.SetVector3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		mat.SetFloat("material.shininess", 132.0f);

		mat.SetVector3("viewPos", mainCamera->GetCameraPos());

		mat.SetFloat("mixValue", materialValue1);
		mat.SetMatrix4("view", 1, GL_FALSE, view);
		mat.SetMatrix4("projection", 1, GL_FALSE, proj);

		int i = 0;
		
		for (auto &box : boxes) {
			// calculate the model matrix for each object and pass it to shader before drawing
			float angle = 20.0f * i;
			if (i % 3 == 0)  // every 3rd iteration (including the first) we set the angle using GLFW's time function.
				angle = (float)glfwGetTime() * 25.0f;
			box.ChangeScale(glm::vec3(2));
			box.ChangeRot(angle, glm::vec3(0, 1, 0));
			mat.SetMatrix4("model", 1, GL_FALSE, box.GetTransform());
			box.Draw();
			

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
	glDeleteVertexArrays(1, &boxVao);
	glDeleteBuffers(1, &boxVbo);
	glDeleteVertexArrays(1, &lightVao);
	glDeleteBuffers(1, &lightVbo);
	delete scene;
}
