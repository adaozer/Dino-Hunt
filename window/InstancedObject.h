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

	void setInstances(std::vector<INSTANCE>& _instances) {
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
			shaderManager->updateTexturePS(core, "tex", texture->heapOffset);
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
	Texture* texture = nullptr;
	std::string filepath;

	InstancedObject(ShaderManager* shadermanager, TextureManager* tx, std::string _filepath) : shaderManager(shadermanager), textureManager(tx), filepath(_filepath) {}

	void init(Core* core, std::string filename, std::vector<INSTANCE>& instances) {
		sm.setInstances(instances);
		sm.load(core, filename);

		vertexShader = shaderManager->loadShader(core, "vertexshader_instanced.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader_textured.hlsl", false);
		texture = textureManager->loadTexture(core, filepath);
		psos.createPSO(core, "InstancedGEM", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getInstancedLayout());
	}

	void draw(Core* core, Matrix& VP) {
		psos.bind(core, "InstancedGEM");
		shaderManager->updateConstantVS("vertexshader_instanced.hlsl", "staticMeshBuffer", "VP", &VP);
		vertexShader->apply(core);
		sm.draw(core, texture, shaderManager);
	}
};