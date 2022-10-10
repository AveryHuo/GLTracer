#include "Scene.h"

Scene::Scene()
{
	//Default value
	width = 800.0f;
	height = 600.0f;
}

void Scene::SetSceneSize(float w, float h)
{
	width = w; 
	height = h; 

	for (auto iter = cameras.begin(); iter != cameras.end(); iter++) {
		(*iter)->SetCameraRatio(w/h);
	}
}

void Scene::AddLight(glm::vec3 initPos, Material* material)
{
	Light l = Light();
	l.ChangePos(initPos);
	l.SetMaterial(material);
	lights.push_back(l);
}

void Scene::AddBox(glm::vec3 initPos, Material* material)
{
	Box b = Box();
	b.ChangePos(initPos);
	b.SetMaterial(material);
	boxes.push_back(b);
}

void Scene::AddSphere(glm::vec3 initPos, Material* material)
{
	Sphere s = Sphere();
	s.ChangePos(initPos);
	s.SetMaterial(material);
	spheres.push_back(s);
}

void Scene::AddCylinder(glm::vec3 initPos, Material* material)
{
	Cylinder c = Cylinder();
	c.ChangePos(initPos);
	c.SetMaterial(material);
	cylinders.push_back(c);
}

void Scene::AddQuad(glm::vec3 initPos, Material* material)
{
	Quad c = Quad();
	c.ChangePos(initPos);
	c.SetMaterial(material);
	quads.push_back(c);
}

Texture *Scene::AddTexture(const int channel, const std::string path, const GLint colorRange)
{
	return AddTexture(channel, path, colorRange, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);
}

Texture *Scene::AddTexture(const int channel, const std::string path, const GLint colorRange, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter)
{
	Texture *texture = new Texture(path, colorRange, wrapS, wrapT, minFilter, magFilter);
	if (texture->IsLoadSuccess()) {
		textureMap.insert({ channel, texture });
		return texture;
	}
	return nullptr;
}

Material* Scene::AddMaterial(const std::string vertShaderName, const std::string fragShaderName)
{
	Material* ret = new Material();
	ret->CreateInstance();
	ret->AttachVertexShaderByName(vertShaderName);
	ret->AttachFragmentShaderByName(fragShaderName);
	materials.push_back(ret);
	return ret;
}

Camera* Scene::AddCamera(const float speed, bool isMainCamera)
{
	Camera *cam = new Camera();
	cam->SetCameraSpeed(speed);
	cam->SetCameraRatio(width / height);
	if (isMainCamera) {
		mainCamera = cam;
	}
	cameras.push_back(cam);
	return cam;
}

void Scene::Load()
{
	for (auto& light : lights) {
		light.init();
	}

	for (auto& box : boxes) {
		box.init();
	}

	for (auto& sphere : spheres) {
		sphere.init();
	}

	for (auto& cylinder : cylinders) {
		cylinder.init();
	}

	for (auto& quad : quads) {
		quad.init();
	}

	AttachTextureToMaterial();
}

void Scene::Unload()
{
	for (auto& light : lights) {
		light.unInit();
	}

	for (auto& box : boxes) {
		box.unInit();
	}

	for (auto& sphere : spheres) {
		sphere.unInit();
	}

	for (auto& cylinder : cylinders) {
		cylinder.unInit();
	}

	for (auto& quad : quads) {
		quad.unInit();
	}
}

void Scene::AttachTextureToMaterial() {

	auto mats = GetAllMaterials();
	auto textures = GetTextureMap();
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


Scene::~Scene()
{
	for (auto iter = materials.begin(); iter != materials.end(); iter++) {
		(* iter)->DestroyInstance();
		delete (* iter);
	}
	materials.clear();

	for (auto iter = textureMap.begin(); iter != textureMap.end(); iter++) {
		delete (*iter).second;
	}
	textureMap.clear();

	lights.clear();
	spheres.clear();
	boxes.clear();
	mainCamera = nullptr;

	for (auto iter = cameras.begin(); iter != cameras.end(); iter++) {
		delete (*iter);
	}
	cameras.clear();
}
