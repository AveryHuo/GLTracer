#pragma once
#include "Texture.h"
#include "DirectionLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Quad.h"
#include "Box.h"
#include "Material.h"
#include "Scene.h"


class MaterialHelper
{
public:
    static void AddLightsToMaterial(Scene *scene, Material* mat) {

        std::vector<DirectionLight*> dirLights = scene->GetDirLights();
        if (dirLights.size() > 0){
            auto dirLight = dirLights[0];
            dirLight->applyToMaterial(mat);
        }

        std::vector<SpotLight*> spotLights = scene->GetSpotLights();
        if (spotLights.size() > 0) {
            for (int i = 0; i < spotLights.size(); i++) {
                auto light = spotLights[i];
                light->applyToMaterial(mat, i);
            }
        }

        std::vector<PointLight*> pointLights = scene->GetPointLights();
        if (pointLights.size() > 0) {
            for (int i = 0; i < pointLights.size(); i++) {
                auto light = pointLights[i];
                light->applyToMaterial(mat, i);
            }
        }
	}
};

