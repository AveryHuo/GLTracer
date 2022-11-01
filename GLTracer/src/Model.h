#pragma once
#include "Material.h"
#include "Mesh.h"
#include "stb_image.h"
#include "Texture.h"
#include "BaseModel.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model:public BaseModel
{
public:
	Model(std::string const& path, bool gamma =false):gammaCorrection(gamma)
	{
		loadModel(path);
	}
	void init() override {};
	void unInit() override{};
	void draw() override;
	void draw(Material & mat);
private:
	std::vector<Texture *> textures_loaded;
	// model data
	std::vector<Mesh> meshes;
	std::string directory;
    bool gammaCorrection;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture *> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
};

