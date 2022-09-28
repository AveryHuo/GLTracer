#pragma once
#include <vector>
#include <string>
#include <map>
#include <glad/glad.h>
#include "Texture.h"
#include "Box.h"
#include "Material.h"
#include "Camera.h"


class Scene
{
private:
    std::vector<Material*> materials;
    std::map<int, Texture*> textureMap;
    std::map<Material, std::vector<Box>> boxMap;
    std::vector<Box> boxes;
    Camera * mainCamera;
    std::vector<Camera *> cameras;
    float width;
    float height;
public:
Scene();

void SetSceneSize(float w, float h){width = w; height = h;}

const std::map<int, Texture*> GetTextureMap(){return textureMap;};
const std::vector<Material*> GetAllMaterials() { return materials; };
const std::map<Material, std::vector<Box>> GetBoxMap() { return boxMap; };
const std::vector<Box> GetBoxs() { return boxes; };
const std::vector<Camera *> GetCameras() { return cameras; };
Camera* GetMainCamera() const {return mainCamera;};

void AddBox(glm::vec3 initPos, const Material &material);
Texture* AddTexture(const int channel, const std::string path, const GLint colorRange);
Texture* AddTexture(const int channel, const std::string path, const GLint colorRange, GLint wrapS, GLint wrapT, GLint minFilter, GLint magFilter);
Material* AddMaterial(const std::string vertShaderName, const std::string fragShaderName);
Camera *AddCamera(const float speed, const bool isMainCamera);
~Scene();
};

