#pragma once
#include "mesh.h"
#include "PSOManager.h"
#include "ShaderManager.h"

class Plane {
public:
	Mesh mesh;
	ShaderManager* shaderManager;
	PSOManager psos;
	VertexLayoutCache vertexLayoutCache;
	Shader* vertexShader = nullptr;
	Shader* pixelShader = nullptr;
	
	Plane(ShaderManager* sm) : shaderManager(sm) {}

	void init(Core* core) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-10, 0, -10), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(10, 0, -10), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-10, 0, 10), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(10, 0, 10), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
		vertexShader = shaderManager->loadShader(core, "vertexshader.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader.hlsl", false);
		psos.createPSO(core, "Plane", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getStaticLayout());

	}

	void draw(Core* core, Matrix& W, Matrix& VP) {
		psos.bind(core, "Plane");

		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "W", &W);
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "VP", &VP);

		vertexShader->apply(core);
		mesh.draw(core);
	}
};
