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

DirectionLight* Scene::AddDirectionLight(glm::vec3 initPos)
{
	DirectionLight *l = new DirectionLight();
	l->ChangePos(initPos);
	l->SetMaterial(AddMaterial("light_vec", "light_frag"));
	dirLights.push_back(l);
	return dirLights[dirLights.size() - 1];;
}


PointLight* Scene::AddPointLight(glm::vec3 initPos)
{
	PointLight *l = new PointLight();
	l->ChangePos(initPos);
	l->SetMaterial(AddMaterial("light_vec", "light_frag"));
	pointLights.push_back(l);
	return pointLights[pointLights.size() - 1];;
}

SpotLight* Scene::AddSpotLight(glm::vec3 initPos)
{
	SpotLight *l = new SpotLight();
	l->ChangePos(initPos);
	l->SetMaterial(AddMaterial("light_vec", "light_frag"));
	spotLights.push_back(l);
	return spotLights[spotLights.size() - 1];
}

Box* Scene::AddBox(glm::vec3 initPos, Material* material)
{
	Box *b = new Box();
	b->ChangePos(initPos);
	b->SetMaterial(material);
	boxes.push_back(b);
	return boxes[boxes.size() - 1];
}

Sphere* Scene::AddSphere(glm::vec3 initPos, Material* material)
{
	Sphere *s = new Sphere();
	s->ChangePos(initPos);
	s->SetMaterial(material);
	spheres.push_back(s);
	return spheres[spheres.size() - 1];
}

Cylinder* Scene::AddCylinder(glm::vec3 initPos, Material* material)
{
	Cylinder *c = new Cylinder();
	c->ChangePos(initPos);
	c->SetMaterial(material);
	cylinders.push_back(c);
	return cylinders[cylinders.size()-1];
}

Quad* Scene::AddQuad(glm::vec3 initPos, Material* material)
{
	Quad *c = new Quad();
	c->ChangePos(initPos);
	c->SetMaterial(material);
	quads.push_back(c);
	return quads[quads.size()-1];
}

Texture *Scene::AddTexture(const int channel, const std::string path)
{
	Texture* texture = new Texture(path);
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

void Scene::Reload() {
	this->Unload();
	this->Load();
}

void Scene::Load()
{
	for (auto& light : dirLights) {
		light->init();
	}
	for (auto& light : pointLights) {
		light->init();
	}
	for (auto& light : spotLights) {
		light->init();
	}

	for (auto& box : boxes) {
		box->init();
	}

	for (auto& sphere : spheres) {
		sphere->init();
	}

	for (auto& cylinder : cylinders) {
		cylinder->init();
	}

	for (auto& quad : quads) {
		quad->init();
	}
}

void Scene::Unload()
{
	for (auto& light : dirLights) {
		light->unInit();
	}
	for (auto& light : pointLights) {
		light->unInit();
	}
	for (auto& light : spotLights) {
		light->unInit();
	}

	for (auto& box : boxes) {
		box->unInit();
	}

	for (auto& sphere : spheres) {
		sphere->unInit();
	}

	for (auto& cylinder : cylinders) {
		cylinder->unInit();
	}

	for (auto& quad : quads) {
		quad->unInit();
	}
}

void Scene::InitMaterialTextures() {

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
				mat->SetTextureSampler("material.diffuse", 0);
			}
			else if (textChannel == GL_TEXTURE1) {
				mat->SetTextureSampler("material.specular", 1);
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

	for (auto& light : dirLights) {
		delete light;
	}
	for (auto& light : pointLights) {
		delete light;
	}
	for (auto& light : spotLights) {
		delete light;
	}

	for (auto& box : boxes) {
		delete box;
	}

	for (auto& sphere : spheres) {
		delete sphere;
	}

	for (auto& cylinder : cylinders) {
		delete cylinder;
	}

	for (auto& quad : quads) {
		delete quad;
	}

	dirLights.clear();
	pointLights.clear();
	spotLights.clear();
	spheres.clear();
	boxes.clear();
	quads.clear();
	mainCamera = nullptr;

	for (auto iter = cameras.begin(); iter != cameras.end(); iter++) {
		delete (*iter);
	}
	cameras.clear();
}
