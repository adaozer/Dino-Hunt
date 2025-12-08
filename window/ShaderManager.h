#pragma once
#include "core.h"
#include "PSOManager.h"
#include "ConstantBuffer.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

class Shader {
public:
	ID3DBlob* shader = nullptr;
	bool isVertexShader = false;
	ConstantBuffer* buffer = nullptr;

	void apply(Core* core) {
		if (buffer == nullptr) return;
		int index = (isVertexShader == true) ? 0 : 1;
		core->getCommandList()->SetGraphicsRootConstantBufferView(index, buffer->getGPUAddress());
		buffer->next();
	}
};

class ShaderManager {
public:
	std::unordered_map<std::string, Shader> shaders;
	std::map<std::string, int> textureBindPoints;

	void reflect(Core* core, ID3DBlob* shader, ConstantBuffer* buffer) {
		ID3D12ShaderReflection* reflection;
		D3DReflect(shader->GetBufferPointer(), shader->GetBufferSize(), IID_PPV_ARGS(&reflection));
		D3D12_SHADER_DESC desc;
		reflection->GetDesc(&desc);
		for (int i = 0; i < desc.ConstantBuffers; i++) {
			ID3D12ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
			D3D12_SHADER_BUFFER_DESC cbDesc;
			constantBuffer->GetDesc(&cbDesc);
			buffer->name = cbDesc.Name;
			for (int j = 0; j < cbDesc.Variables; j++) {
				ID3D12ShaderReflectionVariable* var = constantBuffer->GetVariableByIndex(j);
				D3D12_SHADER_VARIABLE_DESC vDesc;
				var->GetDesc(&vDesc);
				ConstantBufferVariable bufferVariable;
				bufferVariable.offset = vDesc.StartOffset;
				bufferVariable.size = vDesc.Size;
				buffer->constantBufferData.insert({ vDesc.Name, bufferVariable });
			}
			buffer->init(core, cbDesc.Size);
			break;
		}
		for (int i = 0; i < desc.BoundResources; i++) {
			D3D12_SHADER_INPUT_BIND_DESC bindDesc;
			reflection->GetResourceBindingDesc(i, &bindDesc);
			if (bindDesc.Type == D3D_SIT_TEXTURE) textureBindPoints.insert({ bindDesc.Name, bindDesc.BindPoint });
		}
		reflection->Release();

	}

	Shader* loadShader(Core* core, std::string filename, bool isVertexShader) {
		auto it = shaders.find(filename);
		if (it != shaders.end()) {
			return &it->second;
		}

		ID3DBlob* status = nullptr;
		ID3DBlob* shader = nullptr;
		std::string shaderType = (isVertexShader == true) ? "VS" : "PS";
		std::string file = readFile(filename);
		std::string shaderEnd = (isVertexShader == true) ? "vs_5_0" : "ps_5_0";

		HRESULT hr = D3DCompile(file.c_str(), strlen(file.c_str()), NULL,
			NULL, NULL, shaderType.c_str(), shaderEnd.c_str(), 0, 0, &shader, &status);

		if (FAILED(hr)) (char*)status->GetBufferPointer();

		ConstantBuffer* buffer = new ConstantBuffer();
		reflect(core, shader, buffer);
		Shader s;
		s.shader = shader;
		s.isVertexShader = isVertexShader;
		s.buffer = buffer;
		shaders.insert({filename, s });
		return &shaders[filename];
	}

	std::string readFile(std::string filename) {
		std::ifstream file(filename);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}

	void updateConstantVS(const std::string& filename, const std::string& cbufferName, const std::string& varName, const void* data) {
		auto it = shaders.find(filename);
		if (it == shaders.end()) return;

		Shader& sh = it->second;
		if (!sh.isVertexShader) return;
		if (!sh.buffer) return;
		if (sh.buffer->name != cbufferName) return;
		sh.buffer->update(varName, (void*)data);
	}

	void updateConstantPS(const std::string& filename, const std::string& cbufferName, const std::string& varName, const void* data) {
		auto it = shaders.find(filename);
		if (it == shaders.end()) return;

		Shader& sh = it->second;
		if (sh.isVertexShader) return;
		if (!sh.buffer) return;
		if (sh.buffer->name != cbufferName) return;
		sh.buffer->update(varName, (void*)data);
	}

	void updateTexturePS(Core* core, std::string name, int heapOffset) {
		UINT bindPoint = textureBindPoints[name];
		D3D12_GPU_DESCRIPTOR_HANDLE handle = core->srvHeap.gpuHandle;
		handle.ptr = handle.ptr + (UINT64)(heapOffset - bindPoint) * (UINT64)core->srvHeap.incrementSize;
		core->getCommandList()->SetGraphicsRootDescriptorTable(2, handle);
	}
};