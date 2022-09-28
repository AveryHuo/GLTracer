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

void Scene::AddBox(glm::vec3 initPos,const Material& material)
{
	Box b = Box();
	b.ChangePos(initPos);
	boxes.push_back(b);

	auto boxes = boxMap.find(material);
	if (boxes != boxMap.end()) {
		boxes->second.push_back(b);
	}
	else {
		std::vector<Box> nboxList{b};
		boxMap.insert({material, nboxList });
	}
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

	for (auto iter = boxMap.begin(); iter != boxMap.end(); iter++) {
		(*iter).second.clear();
	}
	boxMap.clear();
	boxes.clear();
	mainCamera = nullptr;

	for (auto iter = cameras.begin(); iter != cameras.end(); iter++) {
		delete (*iter);
	}
	cameras.clear();
}
