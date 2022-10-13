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


class Scene
{
private:
	std::vector<Material*> materials;
	std::map<int, Texture*> textureMap;
	std::vector<Quad*> quads;
	std::vector<Cylinder*> cylinders;
	std::vector<Sphere*> spheres;
	std::vector<Box*> boxes;
	std::vector<DirectionLight*> dirLights;
	std::vector<SpotLight*> spotLights;
	std::vector<PointLight*> pointLights;
	Camera* mainCamera = nullptr;
	std::vector<Camera*> cameras;
	float width;
	float height;

public:
	Scene();

	void SetSceneSize(float w, float h);

	const std::map<int, Texture*> GetTextureMap() { return textureMap; };
	const std::vector<Material*> GetAllMaterials() { return materials; };
	std::vector<Box*> GetBoxs() { return boxes; };
	std::vector<DirectionLight*> GetDirLights() { return dirLights; };
	std::vector<SpotLight*> GetSpotLights() { return spotLights; };
	std::vector<PointLight*> GetPointLights() { return pointLights; };
	std::vector<Sphere*> GetSpheres() { return spheres; };
	std::vector<Cylinder*> GetCylinders() { return cylinders; };
	std::vector<Quad*> GetQuads() { return quads; };
	std::vector<Camera*> GetCameras() { return cameras; };
	Camera* GetMainCamera() const { return mainCamera; };

	DirectionLight* AddDirectionLight(glm::vec3 initPos);
	SpotLight* AddSpotLight(glm::vec3 initPos);
	PointLight* AddPointLight(glm::vec3 initPos);
	Box* AddBox(glm::vec3 initPos, Material* material);
	Sphere* AddSphere(glm::vec3 initPos, Material* material);
	Cylinder* AddCylinder(glm::vec3 initPos, Material* material);
	Quad* AddQuad(glm::vec3 initPos, Material* material);
	Texture* AddTexture(const int channel, const std::string path, const GLint colorRange);
	Texture* AddTexture(const int channel, const std::string path, const GLint colorRange, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	Material* AddMaterial(const std::string vertShaderName, const std::string fragShaderName);
	Camera* AddCamera(const float speed, const bool isMainCamera);

	void Reload();
	void Load();
	void Unload();

	void InitMaterialTextures();
	~Scene();
};

