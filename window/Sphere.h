#pragma once

#include "mesh.h"
#include "ShaderManager.h"
#include "Texture.h"

class Sphere {
public:
	Mesh mesh;
	ShaderManager* shaderManager;
	PSOManager psos;
	VertexLayoutCache vertexLayoutCache;
	Shader* vertexShader = nullptr;
	Shader* pixelShader = nullptr;
	TextureManager* textureManager = nullptr;
	Texture* texture = nullptr;
	std::string filename;

	std::vector<STATIC_VERTEX> vertices;
	std::vector<unsigned int> indices;

	Sphere(ShaderManager* sm, TextureManager* tm, std::string _filename) : shaderManager(sm), textureManager(tm), filename(_filename) {}

	void init(Core* core, float _radius) {
		float radius = _radius;
		int rings = 16;
		int segments = 16;
		float pi = 3.14159f;
		for (int lat = 0; lat <= rings; lat++) {
			float theta = lat * M_PI / rings;
			float sinTheta = sinf(theta);
			float cosTheta = cosf(theta);
			for (int lon = 0; lon <= segments; lon++) {
				float phi = lon * 2.0f * M_PI / segments;
				float sinPhi = sinf(phi);
				float cosPhi = cosf(phi);
				Vec3 position(radius * sinTheta * cosPhi, radius * cosTheta,
					radius * sinTheta * sinPhi);
				Vec3 normal = position.normalize();
				float tu = 1.0f - (float)lon / segments;
				float tv = (float)lat / rings;
				vertices.push_back(addVertex(position, normal, tu, tv));
			}
		}
		for (int lat = 0; lat < rings; lat++)
		{
			for (int lon = 0; lon < segments; lon++)
			{
				int current = lat * (segments + 1) + lon;
				int next = current + segments + 1;
				indices.push_back(current);
				indices.push_back(next);
				indices.push_back(current + 1);
				indices.push_back(current + 1);
				indices.push_back(next);
				indices.push_back(next + 1);
			}
		}
		mesh.init(core, vertices, indices);
		vertexShader = shaderManager->loadShader(core, "vertexshader.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader_textured.hlsl", false);
		texture = textureManager->loadTexture(core, filename);
		psos.createPSO(core, "Sphere", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getStaticLayout());
	}

	void draw(Core* core, Matrix& W, Matrix& VP) {
		psos.bind(core, "Sphere");

		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "W", &W);
		shaderManager->updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "VP", &VP);

		vertexShader->apply(core);
		shaderManager->updateTexturePS(core, "tex", texture->heapOffset);
		mesh.draw(core);
	}
};
