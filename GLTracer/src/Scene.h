#pragma once
#include <vector>
#include <string>
#include <map>
#include <glad/glad.h>
#include "Texture.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Quad.h"
#include "Box.h"
#include "Material.h"
#include "Camera.h"
#include "Model.h"
#include "Skybox.h"
#include "CustomModel.h"
#include "AsterionField.h"

class Scene
{
private:
	const std::string SKYBOX_VERTEX_SHADER = "skybox_vertex_shader";
	const std::string SKYBOX_FRAGMENT_SHADER = "skybox_fragment_shader";
	//TODO: Consider to move all of this resources to a manager? get instance from manager

	GLuint uboMatrices;
	std::vector<Material*> materials;
	std::vector<Cubemap*> cubemaps;
	std::vector<Texture*> textures;
	std::vector<Quad*> quads;
	std::vector<Cylinder*> cylinders;
	std::vector<Sphere*> spheres;
	std::vector<Box*> boxes;
	std::vector<Model*> models;
	std::vector<CustomModel*> customModels;
	std::vector<DirectionLight*> dirLights;
	std::vector<SpotLight*> spotLights;
	std::vector<PointLight*> pointLights;
	
	AsterionField *asterionField = nullptr;
	Camera* mainCamera = nullptr;
	std::vector<Camera*> cameras;
	Skybox *skybox;
	float width;
	float height;

public:
	Scene();

	void SetSceneSize(float w, float h);
	const std::vector<Material*> GetAllMaterials() { return materials; };
	std::vector<Cubemap*> GetCubemaps() { return cubemaps; };
	std::vector<Box*> GetBoxs() { return boxes; };
	std::vector<DirectionLight*> GetDirLights() { return dirLights; };
	std::vector<SpotLight*> GetSpotLights() { return spotLights; };
	std::vector<PointLight*> GetPointLights() { return pointLights; };
	std::vector<Sphere*> GetSpheres() { return spheres; };
	std::vector<Cylinder*> GetCylinders() { return cylinders; };
	std::vector<Quad*> GetQuads() { return quads; };
	std::vector<Model*> GetModels() { return models; };
	std::vector<Camera*> GetCameras() { return cameras; };
	std::vector<CustomModel*> GetCustomModels() { return customModels; };
	Camera* GetMainCamera() const { return mainCamera; };
	Skybox* GetSkybox() const {return skybox;};
	AsterionField *GetAsterionField() const {return asterionField;};
	
	DirectionLight* AddDirectionLight(glm::vec3 initPos);
	SpotLight* AddSpotLight(glm::vec3 initPos);
	PointLight* AddPointLight(glm::vec3 initPos);
	Box* AddBox(glm::vec3 initPos, Material* material);
	Sphere* AddSphere(glm::vec3 initPos, Material* material);
	Cylinder* AddCylinder(glm::vec3 initPos, Material* material);
	Quad* AddQuad(glm::vec3 initPos, Material* material);
	Model* AddModel(std::string path, glm::vec3 initPos, Material* material);
	Model* Add(std::string path, glm::vec3 initPos, Material* material);
	Model* AddModelWithTwoMat(std::string path, glm::vec3 initPos, Material* material1, Material* material2);
	Model* AddModelWithThreeMat(std::string path, glm::vec3 initPos, Material* material1, Material* material2, Material* material3);
	CustomModel* AddCustomModel(Material* material);
	Cubemap* AddCubemap(const std::string path);
	Texture* AddTexture(const std::string path);
	Material* AddMaterial(const std::string vertShaderName, const std::string fragShaderName, const std::string geometryShaderName = "");
	Camera* AddCamera(const float speed, const bool isMainCamera);
	Skybox* AddSkybox(const std::string path);
	AsterionField* AddAsterionField(const std::string& rockPath, const std::string& planetPath);

	void Reload();
	void Load();
	void Unload();

	~Scene();
};

