#pragma once
#include "GEMLoader.h"
#include "mesh.h"

class StaticMesh {
public:
	GEMLoader::GEMModelLoader loader;
	std::vector<GEMLoader::GEMMesh> gemmeshes;
	std::vector<Mesh*> meshes;
	std::vector<STATIC_VERTEX> vertices;

	void load(Core* core, std::string filename) {
		loader.load(filename, gemmeshes);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh* mesh = new Mesh();
			for (int j = 0; j < gemmeshes[i].verticesStatic.size(); j++) {
				STATIC_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesStatic[j], sizeof(STATIC_VERTEX));
				vertices.push_back(v);
			}
			mesh->init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
	}

	void draw(Core* core) {
		for (int i = 0; i < meshes.size(); i++) {
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

	GEMObject(ShaderManager* shadermanager) : shaderManager(shadermanager) {}

	void init(Core* core, std::string filename) {
		sm.load(core, filename);
		vertexShader = shaderManager->loadShader(core, "vertexshader.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader.hlsl", false);
		psos.createPSO(core, "GEMObject", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getStaticLayout());
	}

	void draw(Core* core, Matrix& W, Matrix& VP) {
		core->beginRenderPass();
		psos.bind(core, "GEMObject");
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "W", &W);
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "VP", &VP);
		vertexShader->apply(core);
		sm.draw(core);
	}
};