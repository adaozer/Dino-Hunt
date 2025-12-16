#pragma once
#include "mesh.h"
#include "PSOManager.h"
#include "ShaderManager.h"
#include "Texture.h"

class Plane {
public:
	Mesh mesh;
	ShaderManager* shaderManager;
	PSOManager psos;
	VertexLayoutCache vertexLayoutCache;
	Shader* vertexShader = nullptr;
	Shader* pixelShader = nullptr;
	TextureManager* textureManager = nullptr;
	Texture* diffuseTex = nullptr;
	Texture* normalTex = nullptr;
	std::string filename;
	std::string filename2;

	Plane(ShaderManager* sm, TextureManager* tm, std::string _filename, std::string _filename2) : shaderManager(sm), textureManager(tm),
		filename(_filename), filename2(_filename2) {}

	void init(Core* core) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-1000, 0, -1000), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(1000, 0, -1000), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-1000, 0, 1000), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(1000, 0, 1000), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
		vertexShader = shaderManager->loadShader(core, "vertexshader.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader_normalmapped.hlsl", false);
		diffuseTex = textureManager->loadTexture(core, filename);
		normalTex = textureManager->loadTexture(core, filename2, true);
		psos.createPSO(core, "Plane", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getStaticLayout());

	}

	void draw(Core* core, Matrix& W, Matrix& VP) {
		psos.bind(core, "Plane");
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "W", &W);
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "VP", &VP);
		vertexShader->apply(core);
		shaderManager->updateTexturePS(core, "diffuseTex", diffuseTex->heapOffset);
		mesh.draw(core);
	}
};
