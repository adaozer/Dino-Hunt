#pragma once
#include "GEMLoader.h"
#include "mesh.h"
#include "Texture.h"

class StaticMesh {
public:
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	std::vector<Mesh*> meshes;

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
			mesh->init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
	}

	void draw(Core* core, Texture* diffuseTex, ShaderManager* shaderManager) {
		for (int i = 0; i < meshes.size(); i++) {
			shaderManager->updateTexturePS(core, "diffuseTex", diffuseTex->heapOffset);
			meshes[i]->draw(core);
		}
	}
};

class GEMObject {
public:
	ShaderManager* shaderManager;
	PSOManager psos;
	VertexLayoutCache vertexLayoutCache;
	Shader* vertexShader = nullptr;
	Shader* pixelShader = nullptr;
	StaticMesh sm;
	TextureManager* textureManager;
	Texture* diffuseTex = nullptr;
	Texture* normalTex = nullptr;
	std::string filepath;
	std::string filepath2;

	GEMObject(ShaderManager* shadermanager, TextureManager* tx, std::string _filepath, std::string _filepath2) : shaderManager(shadermanager), textureManager(tx), 
		filepath(_filepath), filepath2(_filepath2) {}

	void init(Core* core, std::string filename) {
		sm.load(core, filename);
		vertexShader = shaderManager->loadShader(core, "vertexshader.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader_normalmapped.hlsl", false);
		diffuseTex = textureManager->loadTexture(core, filepath);
		normalTex = textureManager->loadTexture(core, filepath2);
		psos.createPSO(core, "GEMObject", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getStaticLayout());
	}

	void draw(Core* core, Matrix& W, Matrix& VP) {
		psos.bind(core, "GEMObject");
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "W", &W);
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "VP", &VP);
		vertexShader->apply(core);
		sm.draw(core, diffuseTex, shaderManager);
	}
};