#pragma once
#include "Animation.h"
#include "GEMLoader.h"
#include "ShaderManager.h"
#include "texture.h"

class AnimatedModel {
public:
	std::vector<Mesh*> meshes;
	Animation animation;

	ShaderManager* shaderManager;
	PSOManager psos;
	VertexLayoutCache vertexLayoutCache;
	Shader* vertexShader = nullptr;
	Shader* pixelShader = nullptr;

	GEMLoader::GEMAnimation anim;
	std::vector<std::string> textureFilenames;

	TextureManager* textureManager;
	Texture* diffuseTex = nullptr;
	Texture* normalTex = nullptr;
	std::string filepath;
	std::string filepath2;

	AnimatedModel(ShaderManager* sm, TextureManager* tx, std::string _filepath, std::string _filepath2) : shaderManager(sm), 
		textureManager(tx), filepath(_filepath), filepath2(_filepath2) {}
	void load(Core* core, std::string filename)
	{
		GEMLoader::GEMModelLoader loader;
		std::vector<GEMLoader::GEMMesh> gemmeshes;
		GEMLoader::GEMAnimation gemanimation;
		loader.load(filename, gemmeshes, gemanimation);
		for (int i = 0; i < gemmeshes.size(); i++) {
			Mesh* mesh = new Mesh;
			std::vector<ANIMATED_VERTEX> vertices;
			for (int j = 0; j < gemmeshes[i].verticesAnimated.size(); j++) {
				ANIMATED_VERTEX v;
				memcpy(&v, &gemmeshes[i].verticesAnimated[j], sizeof(ANIMATED_VERTEX));
				vertices.push_back(v);
			}
			textureFilenames.push_back(gemmeshes[i].material.find("albedo").getValue());
			mesh->init(core, vertices, gemmeshes[i].indices);
			meshes.push_back(mesh);
		}
		vertexShader = shaderManager->loadShader(core, "vertexshader_animated.hlsl", true);
		pixelShader = shaderManager->loadShader(core, "pixelshader_alphatesting.hlsl", false);
		diffuseTex = textureManager->loadTexture(core, filepath);
		//normalTex = textureManager->loadTexture(core, filepath2);
		psos.createPSO(core, "AnimatedModel", vertexShader->shader, pixelShader->shader, vertexLayoutCache.getAnimatedLayout());
		memcpy(&animation.skeleton.globalInverse, &gemanimation.globalInverse, 16 * sizeof(float));
		for (int i = 0; i < gemanimation.bones.size(); i++)
		{
			Bone bone;
			bone.name = gemanimation.bones[i].name;
			memcpy(&bone.offset, &gemanimation.bones[i].offset, 16 * sizeof(float));
			bone.parentIndex = gemanimation.bones[i].parentIndex;
			animation.skeleton.bones.push_back(bone);
		}
		for (int i = 0; i < gemanimation.animations.size(); i++)
		{
			std::string name = gemanimation.animations[i].name;
			AnimationSequence aseq;
			aseq.ticksPerSecond = gemanimation.animations[i].ticksPerSecond;
			for (int n = 0; n < gemanimation.animations[i].frames.size(); n++)
			{
				AnimationFrame frame;
				for (int index = 0; index < gemanimation.animations[i].frames[n].positions.size(); index++)
				{
					Vec3 p;
					Quaternion q;
					Vec3 s;
					memcpy(&p, &gemanimation.animations[i].frames[n].positions[index], sizeof(Vec3));
					frame.positions.push_back(p);
					memcpy(&q, &gemanimation.animations[i].frames[n].rotations[index], sizeof(Quaternion));
					frame.rotations.push_back(q);
					memcpy(&s, &gemanimation.animations[i].frames[n].scales[index], sizeof(Vec3));
					frame.scales.push_back(s);
				}
				aseq.frames.push_back(frame);
			}
			animation.animations.insert({ name, aseq });
		}
		anim = gemanimation;
	}

	void draw(Core* core, AnimationInstance* instance, Matrix& W, Matrix& VP)
	{
		psos.bind(core, "AnimatedModel");
		shaderManager->updateConstantVS("vertexshader_animated.hlsl", "staticMeshBuffer", "W", &W);
		shaderManager->updateConstantVS("vertexshader_animated.hlsl", "staticMeshBuffer", "VP", &VP);
		shaderManager->updateConstantVS("vertexshader_animated.hlsl", "staticMeshBuffer", "bones", instance->matrices);
		Vec3 ld = Vec3(1, 1, 0).normalize();
		Vec3 lc = Vec3(1, 1, 1);
		//shaderManager->updateConstantPS("pixelshader_normalmapped.hlsl", "Lights", "lightDir", &ld);
		//shaderManager->updateConstantPS("pixelshader_normalmapped.hlsl", "Lights", "lightColour", &lc);
		vertexShader->apply(core);
		//pixelShader->apply(core);

		for (int i = 0; i < meshes.size(); i++) {
			shaderManager->updateTexturePS(core, "diffuseTex", diffuseTex->heapOffset);
			meshes[i]->draw(core);
		}
	}
};
