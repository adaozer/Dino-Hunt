#pragma once
#include "GEMLoader.h"
#include "mesh.h"
#include "Texture.h"

class InstancedStaticMesh {
public:
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	std::vector<Mesh*> meshes;
	std::vector<INSTANCE> instances;

	void setInstances(const std::vector<INSTANCE>& _instances) {
		instances = _instances;
	}

	void load(Core* core, std::string filename) {
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh* mesh = new Mesh();
			std::vector<STATIC_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}
			mesh->init(core, vertices, instances, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
	}

	void draw(Core* core, Texture* texture, ShaderManager* shaderManager) {
		for (int i = 0; i < meshes.size(); i++) {
			shaderManager->updateTexturePS(core, "diffuseTex", texture->heapOffset);
			meshes[i]->draw(core);
		}
	}
};

class InstancedObject {
public:
	ShaderManager* shaderManager;
	PSOManager psos;
	VertexLayoutCache vertexLayoutCache;
	Shader* vertexShader = nullptr;
	Shader* pixelShader = nullptr;
	InstancedStaticMesh sm;
	TextureManager* textureManager;
	Texture* diffuseTex = nullptr;
	Texture* normalTex = nullptr;
	std::string filepath;
	std::string filepath2;

	InstancedObject(ShaderManager* shadermanager, TextureManager* tx, std::string _filepath, std::string _filepath2) : shaderManager(shadermanager), 
		textureManager(tx), filepath(_filepath), filepath2(_filepath2) {}

	void init(Core* core, std::string filename, std::vector<INSTANCE>& instances) {
		sm.setInstances(instances);
		sm.load(core, filename);

		vertexShader = shaderManager->loadShader(core, "vertexshader_instanced.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader_alphatesting.hlsl", false);
		diffuseTex = textureManager->loadTexture(core, filepath);
		//normalTex = textureManager->loadTexture(core, filepath2);
		psos.createPSO(core, "InstancedGEM", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getInstancedLayout());
	}

	void draw(Core* core, Matrix& VP) {
		psos.bind(core, "InstancedGEM");
		shaderManager->updateConstantVS("vertexshader_instanced.hlsl", "staticMeshBuffer", "VP", &VP);
		vertexShader->apply(core);
		sm.draw(core, diffuseTex, shaderManager);
	}
};