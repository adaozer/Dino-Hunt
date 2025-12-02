#pragma once
#include "core.h"
#include <string>
#include "pso.h"
#include <fstream>
#include "buffering.h"
#include <sstream>

class Shader {
public:
	ID3DBlob* shader = nullptr;
	bool isVertexShader = false;
	std::vector<ConstantBuffer> vsConstantBuffers;
	std::vector<ConstantBuffer> psConstantBuffers;

	void apply(Core* core) {
		if (!vsConstantBuffers.empty()) {
			core->getCommandList()->SetGraphicsRootConstantBufferView(0, vsConstantBuffers[0].getGPUAddress());
			vsConstantBuffers[0].next();
		}
		if (!psConstantBuffers.empty()) {
			core->getCommandList()->SetGraphicsRootConstantBufferView(1, psConstantBuffers[0].getGPUAddress());
			psConstantBuffers[0].next();
		}
	}

	void* GetBufferPointer() { return shader ? shader->GetBufferPointer() : nullptr; }

	SIZE_T GetBufferSize() { return shader ? shader->GetBufferSize() : 0; }

};

class ShaderManager {
public:
	ID3DBlob* vertexShader;
	ID3DBlob* pixelShader;
	PSOManager psos;
	std::map<std::string, Shader> shaders;

	void reflect(Core* core, Shader* shader) {
		ID3D12ShaderReflection* reflection;
		D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_PPV_ARGS(&reflection));
		D3D12_SHADER_DESC desc;
		reflection->GetDesc(&desc);
		for (int i = 0; i < desc.ConstantBuffers; i++) {
			ConstantBuffer buffer;
			ID3D12ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
			D3D12_SHADER_BUFFER_DESC cbDesc;
			constantBuffer->GetDesc(&cbDesc);
			buffer.name = cbDesc.Name;
			unsigned int totalSize = 0;
			for (int j = 0; j < cbDesc.Variables; j++) {
				ID3D12ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(j);
				D3D12_SHADER_VARIABLE_DESC vDesc;
				var->GetDesc(&vDesc);
				ConstantBufferVariable bufferVariable;
				bufferVariable.offset = vDesc.StartOffset;
				bufferVariable.size = vDesc.Size;
				buffer.constantBufferData.insert({ vDesc.Name, bufferVariable });
				totalSize += bufferVariable.size;
			}
			buffer.init(core, cbDesc.Size);
			if (shader->isVertexShader) {
				shader->vsConstantBuffers.push_back(buffer);
			}
			else {
				shader->psConstantBuffers.push_back(buffer);
			}
		}
		reflection->Release();
	}

	void init(Core* core, const D3D12_INPUT_LAYOUT_DESC& inputLayout) {
		ID3DBlob* status;
		std::string vertexshader = readFile("vertexshader.hlsl");
		HRESULT hr = D3DCompile(vertexshader.c_str(), strlen(vertexshader.c_str()), NULL,
			NULL, NULL, "VS", "vs_5_0", 0, 0, &vertexShader, &status);
		std::string pixelshader = readFile("pixelshader.hlsl");
		hr = D3DCompile(pixelshader.c_str(), strlen(pixelshader.c_str()), NULL, NULL,
			NULL, "PS", "ps_5_0", 0, 0, &pixelShader, &status);

		if (FAILED(hr)) (char*)status->GetBufferPointer();
		psos.createPSO(core, "Triangle", vertexShader, pixelShader, inputLayout);

		Shader vs;
		vs.shader = vertexShader;
		vs.isVertexShader = true;
		reflect(core, &vs);
		shaders["VS"] = vs;

		Shader ps;
		ps.shader = pixelShader;
		ps.isVertexShader = false;
		reflect(core, &ps);
		shaders["PS"] = ps;
	}

	std::string readFile(std::string filename) {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	void updateConstantVS(const std::string& shaderName, const std::string& cbufferName, const std::string& varName, const void* data) {
		Shader& sh = shaders[shaderName];

		for (auto& cb : sh.vsConstantBuffers) {
			if (cb.name == cbufferName) {
				cb.update(varName, (void*)data);
				return;
			}
		}
	}

	void updateConstantPS(const std::string& shaderName, const std::string& cbufferName, const std::string& varName, const void* data) {
		Shader& sh = shaders[shaderName];

		for (auto& cb : sh.psConstantBuffers) {
			if (cb.name == cbufferName) {
				cb.update(varName, (void*)data);
				return;
			}
		}
	}

};