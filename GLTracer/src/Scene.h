#pragma once
#include <vector>
#include <string>
#include <map>
#include <glad/glad.h>
#include "Texture.h"
#include "Light.h"
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
	std::vector<Quad> quads;
	std::vector<Cylinder> cylinders;
	std::vector<Sphere> spheres;
	std::vector<Box> boxes;
	std::vector<Light> lights;
	Camera* mainCamera = nullptr;
	std::vector<Camera*> cameras;
	float width;
	float height;

	void AttachTextureToMaterial();
public:
	Scene();

	void SetSceneSize(float w, float h);

	const std::map<int, Texture*> GetTextureMap() { return textureMap; };
	const std::vector<Material*> GetAllMaterials() { return materials; };
	std::vector<Box> GetBoxs() { return boxes; };
	std::vector<Light> GetLights() { return lights; };
	std::vector<Sphere> GetSpheres() { return spheres; };
	std::vector<Cylinder> GetCylinders() { return cylinders; };
	std::vector<Quad> GetQuads() { return quads; };
	std::vector<Camera*> GetCameras() { return cameras; };
	Camera* GetMainCamera() const { return mainCamera; };

	void AddLight(glm::vec3 initPos, Material* material);
	void AddBox(glm::vec3 initPos, Material* material);
	void AddSphere(glm::vec3 initPos, Material* material);
	void AddCylinder(glm::vec3 initPos, Material* material);
	void AddQuad(glm::vec3 initPos, Material* material);
	Texture* AddTexture(const int channel, const std::string path, const GLint colorRange);
	Texture* AddTexture(const int channel, const std::string path, const GLint colorRange, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
	Material* AddMaterial(const std::string vertShaderName, const std::string fragShaderName);
	Camera* AddCamera(const float speed, const bool isMainCamera);

	void Load();
	void Unload();
	~Scene();
};

