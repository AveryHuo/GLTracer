#include "Renderer.h"
#include <stdio.h>


Renderer::Renderer(GLFWwindow* window, Scene* scene) : vao(0), vbo(0), mainWindow(window), scene(scene), materialValue1(0.2f)
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
