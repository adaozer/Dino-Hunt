#pragma once
#include "core.h"
#include "maths.h"
#include "ShaderManager.h"

struct PRIM_VERTEX
{
	Vec3 position;
	Colour colour;
};

struct STATIC_VERTEX
{
	Vec3 pos;
	Vec3 normal;
	Vec3 tangent;
	float tu;
	float tv;
};

struct Vec2 {
	float x, y;
	Vec2() : x(0), y(0) {}
	Vec2(float _x, float _y) : x(_x), y(_y) {}
};

class VertexLayoutCache {
public:
	static const D3D12_INPUT_LAYOUT_DESC& getStaticLayout() {
		static const D3D12_INPUT_ELEMENT_DESC inputLayoutStatic[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};
		static const D3D12_INPUT_LAYOUT_DESC desc = { inputLayoutStatic, 4 };
		return desc;
	}
};

class Mesh {
public:
	ID3D12Resource* vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW vbView;

	D3D12_INPUT_ELEMENT_DESC inputLayout[2];
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc;

	ID3D12Resource* indexBuffer;
	D3D12_INDEX_BUFFER_VIEW ibView;
	unsigned int numMeshIndices;

	void init(Core* core, void* vertices, int vertexSizeInBytes, int numVertices, unsigned int* indices, int numIndices) {
		D3D12_HEAP_PROPERTIES heapprops = {};
		heapprops.Type = D3D12_HEAP_TYPE_DEFAULT;
		heapprops.CreationNodeMask = 1;
		heapprops.VisibleNodeMask = 1;

		D3D12_RESOURCE_DESC vbDesc = {};
		vbDesc.Width = numVertices * vertexSizeInBytes;
		vbDesc.Height = 1;
		vbDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbDesc.DepthOrArraySize = 1;
		vbDesc.MipLevels = 1;
		vbDesc.SampleDesc.Count = 1;
		vbDesc.SampleDesc.Quality = 0;
		vbDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		D3D12_RESOURCE_DESC ibDesc;
		memset(&ibDesc, 0, sizeof(D3D12_RESOURCE_DESC));
		ibDesc.Width = numIndices * sizeof(unsigned int);
		ibDesc.Height = 1;
		ibDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		ibDesc.DepthOrArraySize = 1;
		ibDesc.MipLevels = 1;
		ibDesc.SampleDesc.Count = 1;
		ibDesc.SampleDesc.Quality = 0;
		ibDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &vbDesc,
			D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&vertexBuffer));
		core->uploadResource(vertexBuffer, vertices, numVertices * vertexSizeInBytes,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		HRESULT hr;
		hr = core->device->CreateCommittedResource(&heapprops, D3D12_HEAP_FLAG_NONE, &ibDesc,
			D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&indexBuffer));
		core->uploadResource(indexBuffer, indices, numIndices * sizeof(unsigned int),
			D3D12_RESOURCE_STATE_INDEX_BUFFER);

		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vbView.StrideInBytes = vertexSizeInBytes;
		vbView.SizeInBytes = numVertices * vertexSizeInBytes;

		ibView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R32_UINT;
		ibView.SizeInBytes = numIndices * sizeof(unsigned int);
		numMeshIndices = numIndices;

		inputLayout[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		inputLayout[1] = { "COLOUR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		inputLayoutDesc.NumElements = 2;
		inputLayoutDesc.pInputElementDescs = inputLayout;
	}

	void init(Core* core, std::vector<STATIC_VERTEX> vertices, std::vector<unsigned int> indices)
	{
		init(core, &vertices[0], sizeof(STATIC_VERTEX), vertices.size(), &indices[0], indices.size());
		inputLayoutDesc = VertexLayoutCache::getStaticLayout();
	}


	void draw(Core* core) {
		core->getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		core->getCommandList()->IASetVertexBuffers(0, 1, &vbView);
		core->getCommandList()->IASetIndexBuffer(&ibView);
		core->getCommandList()->DrawIndexedInstanced(numMeshIndices, 1, 0, 0, 0);
	}

};

class Plane {
public:
	Mesh mesh;
	ShaderManager shaderManager;

	STATIC_VERTEX addVertex(Vec3 p, Vec3 n, float tu, float tv) {
		STATIC_VERTEX v;
		v.pos = p;
		v.normal = n;
		v.tangent = Vec3(0, 0, 0); // For now
		v.tu = tu;
		v.tv = tv;
		return v;
	}

	void init(Core* core) {
		std::vector<STATIC_VERTEX> vertices;
		vertices.push_back(addVertex(Vec3(-15, 0, -15), Vec3(0, 1, 0), 0, 0));
		vertices.push_back(addVertex(Vec3(15, 0, -15), Vec3(0, 1, 0), 1, 0));
		vertices.push_back(addVertex(Vec3(-15, 0, 15), Vec3(0, 1, 0), 0, 1));
		vertices.push_back(addVertex(Vec3(15, 0, 15), Vec3(0, 1, 0), 1, 1));
		std::vector<unsigned int> indices;
		indices.push_back(2); indices.push_back(1); indices.push_back(0);
		indices.push_back(1); indices.push_back(2); indices.push_back(3);
		mesh.init(core, vertices, indices);
		shaderManager.init(core, mesh.inputLayoutDesc);

	}

	void draw(Core* core, const Matrix& planeWorld, const Matrix& vp, float timeSeconds, const Vec2 lights[4]) {
		core->beginRenderPass();

		Shader& vs = shaderManager.shaders["VS"];
		Shader& ps = shaderManager.shaders["PS"];

		Matrix Wgpu = planeWorld;
		Matrix VPgpu = vp;
		Wgpu.transpose();
		VPgpu.transpose();

		shaderManager.updateConstantVS("VS", "staticMeshBuffer", "W", &Wgpu);
		shaderManager.updateConstantVS("VS", "staticMeshBuffer", "VP", &VPgpu);

		shaderManager.updateConstantPS("PS", "bufferName", "time", &timeSeconds);
		shaderManager.updateConstantPS("PS", "bufferName", "lights", lights);

		vs.apply(core);
		ps.apply(core);

		shaderManager.psos.bind(core, "Triangle");
		mesh.draw(core);
	}
};


class Cube {
	Mesh m;
	ShaderManager shaderManager;

	std::vector<STATIC_VERTEX> vertices;
	std::vector<unsigned int> indices;

	STATIC_VERTEX addVertex(const Vec3& p, const Vec3& n, float tu, float tv) {
			STATIC_VERTEX v;
			v.pos = p;
			v.normal = n;
			v.tangent = Vec3(0, 0, 0); // For now
			v.tu = tu;
			v.tv = tv;
			return v;
		}

		void init(Core* core) {
			vertices.clear();
			indices.clear();
			Vec3 p0 = Vec3(-1.0f, -1.0f, -1.0f);
			Vec3 p1 = Vec3(1.0f, -1.0f, -1.0f);
			Vec3 p2 = Vec3(1.0f, 1.0f, -1.0f);
			Vec3 p3 = Vec3(-1.0f, 1.0f, -1.0f);
			Vec3 p4 = Vec3(-1.0f, -1.0f, 1.0f);
			Vec3 p5 = Vec3(1.0f, -1.0f, 1.0f);
			Vec3 p6 = Vec3(1.0f, 1.0f, 1.0f);
			Vec3 p7 = Vec3(-1.0f, 1.0f, 1.0f);

			vertices.push_back(addVertex(p0, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f));
			vertices.push_back(addVertex(p1, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f));
			vertices.push_back(addVertex(p2, Vec3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f));
			vertices.push_back(addVertex(p3, Vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f));
			vertices.push_back(addVertex(p5, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f));
			vertices.push_back(addVertex(p4, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f));
			vertices.push_back(addVertex(p7, Vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f));
			vertices.push_back(addVertex(p6, Vec3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f));
			vertices.push_back(addVertex(p4, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
			vertices.push_back(addVertex(p0, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
			vertices.push_back(addVertex(p3, Vec3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
			vertices.push_back(addVertex(p7, Vec3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f));

			vertices.push_back(addVertex(p1, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f));
			vertices.push_back(addVertex(p5, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f));
			vertices.push_back(addVertex(p6, Vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f));
			vertices.push_back(addVertex(p2, Vec3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f));
			vertices.push_back(addVertex(p3, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f));
			vertices.push_back(addVertex(p2, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f));
			vertices.push_back(addVertex(p6, Vec3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f));
			vertices.push_back(addVertex(p7, Vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f));
			vertices.push_back(addVertex(p4, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f));
			vertices.push_back(addVertex(p5, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f));
			vertices.push_back(addVertex(p1, Vec3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f));
			vertices.push_back(addVertex(p0, Vec3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f));

			indices.push_back(0); indices.push_back(1); indices.push_back(2);
			indices.push_back(0); indices.push_back(2); indices.push_back(3);
			indices.push_back(4); indices.push_back(5); indices.push_back(6);
			indices.push_back(4); indices.push_back(6); indices.push_back(7);
			indices.push_back(8); indices.push_back(9); indices.push_back(10);
			indices.push_back(8); indices.push_back(10); indices.push_back(11);
			indices.push_back(12); indices.push_back(13); indices.push_back(14);
			indices.push_back(12); indices.push_back(14); indices.push_back(15);
			indices.push_back(16); indices.push_back(17); indices.push_back(18);
			indices.push_back(16); indices.push_back(18); indices.push_back(19);
			indices.push_back(20); indices.push_back(21); indices.push_back(22);
			indices.push_back(20); indices.push_back(22); indices.push_back(23);

			m.init(core, vertices, indices);
			shaderManager.init(core, m.inputLayoutDesc);
		}

		void draw(Core* core, const Matrix& planeWorld, const Matrix& vp, float timeSeconds, const Vec2 lights[4]) {
			core->beginRenderPass();

			Shader& vs = shaderManager.shaders["VS"];
			Shader& ps = shaderManager.shaders["PS"];

			Matrix Wgpu = planeWorld;
			Matrix VPgpu = vp;
			Wgpu.transpose();
			VPgpu.transpose();

			shaderManager.updateConstantVS("VS", "staticMeshBuffer", "W", &Wgpu);
			shaderManager.updateConstantVS("VS", "staticMeshBuffer", "VP", &VPgpu);

			shaderManager.updateConstantPS("PS", "bufferName", "time", &timeSeconds);
			shaderManager.updateConstantPS("PS", "bufferName", "lights", lights);

			vs.apply(core);
			ps.apply(core);

			shaderManager.psos.bind(core, "Triangle");
			m.draw(core);
		}
};
