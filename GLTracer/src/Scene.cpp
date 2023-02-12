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

void Scene::RemoveBox(Box *mbox)
{
	for (auto it = boxes.begin(); it != boxes.end();)
	{
		if (*it == mbox) {
			(*it)->unInit();
			it = boxes.erase(it);
		}
		else {
			++it;
		}
	}
	
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

Model* Scene::AddModel(std::string path, glm::vec3 initPos, Material* material)
{
	Model* m = new Model(path);
	m->ChangePos(initPos);
	m->SetMaterial(material);
	models.push_back(m);
	return models[models.size() - 1];
}

void Scene::RemoveModel(Model* model)
{
	for (auto it = models.begin(); it != models.end();)
	{
		if (*it == model) {
			(*it)->unInit();
			models.erase(it);
		}
		else {
			++it;
		}
	}
}

Model* Scene::AddModelWithTwoMat(std::string path, glm::vec3 initPos, Material* material1, Material* material2)
{
	Model* m = new Model(path);
	m->ChangePos(initPos);
	m->SetMaterial(material1);
	m->SetMaterial2(material2);
	models.push_back(m);
	return models[models.size() - 1];
}

Model* Scene::AddModelWithThreeMat(std::string path, glm::vec3 initPos, Material* material1, Material* material2, Material* material3)
{
	Model* m = new Model(path);
	m->ChangePos(initPos);
	m->SetMaterial(material1);
	m->SetMaterial2(material2);
	m->SetMaterial3(material3);
	models.push_back(m);
	return models[models.size() - 1];
}

CustomModel* Scene::AddCustomModel(Material* material)
{
	CustomModel *m = new CustomModel();
	m->SetMaterial(material);
	customModels.push_back(m);
	return customModels[customModels.size() - 1];
}

Texture *Scene::AddTexture(const std::string path)
{
	Texture* texture = new Texture(path);
	if (texture->IsLoadSuccess()) {
		textures.push_back(texture);
		return texture;
	}
	delete texture;
	return nullptr;
}

Material* Scene::AddMaterial(const std::string vertShaderName, const std::string fragShaderName, const std::string geometryShaderName)
{
	Material* ret = new Material();
	ret->CreateInstance();
	ret->AttachShaderByName(vertShaderName, GL_VERTEX_SHADER);
	if (geometryShaderName != "")
		ret->AttachShaderByName(geometryShaderName, GL_GEOMETRY_SHADER);
	ret->AttachShaderByName(fragShaderName, GL_FRAGMENT_SHADER);
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

Cubemap* Scene::AddCubemap(const std::string path) {
	Cubemap *cm = new Cubemap(path);
	cubemaps.push_back(cm);
	return cm;
}

Skybox* Scene::AddSkybox(const std::string path) {
	auto cubemap = AddCubemap(path);
	if (cubemap == nullptr) {
		return nullptr;
	}
	Material * mat = nullptr;
	if (skybox != nullptr) {
		mat = skybox->GetMaterial();
		delete skybox;
	}
	if (mat == nullptr) {
		mat = AddMaterial(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER);
	}
	skybox = new Skybox();
	skybox->SetMaterial(mat);
	mat->SetCubemap("skybox", GL_TEXTURE0, cubemap);
	return skybox;
}

AsterionField* Scene::AddAsterionField(const std::string& rockPath, const std::string& planetPath) {
	asterionField = new AsterionField(rockPath, planetPath);
	return asterionField;
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

	for (auto& model : models) {
		model->init();
	}

	for (auto& model : customModels) {
		model->init();
	}

	if(skybox)
		skybox->init();

	if(asterionField)
		asterionField->init();
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

	for (auto& model : models) {
		model->unInit();
	}

	for (auto& model : customModels) {
		model->unInit();
	}

	if (skybox)
		skybox->unInit();

	if (asterionField)
		asterionField->unInit();
}

Scene::~Scene()
{
	for (auto iter = materials.begin(); iter != materials.end(); iter++) {
		(* iter)->DestroyInstance();
		delete (* iter);
	}
	materials.clear();

	for (auto iter = textures.begin(); iter != textures.end(); iter++) {
		delete (*iter);
	}

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

	for (auto& model : models) {
		delete model;
	}

	for (auto& model : customModels) {
		delete model;
	}

	dirLights.clear();
	pointLights.clear();
	spotLights.clear();
	spheres.clear();
	boxes.clear();
	quads.clear();
	models.clear();
	mainCamera = nullptr;

	for (auto iter = cameras.begin(); iter != cameras.end(); iter++) {
		delete (*iter);
	}
	cameras.clear();

	if(skybox)
		delete skybox;

	if(asterionField)
		delete asterionField;
}
