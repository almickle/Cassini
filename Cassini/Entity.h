#pragma once
#include "Bindables.h"
#include "Graphics.h"
#include "Vertex.h"

class Entity
{
public:
	Entity(Graphics& gfx)
	{
		LoadMesh("Models\\teapot.txt");
		vertexBuffer = new VertexBuffer(gfx, vertices);
		indexBuffer = new IndexBuffer(gfx, indices);
		topology = new Topology();
		instanceBuffer = new ConstantBuffer(gfx);
		//rasterizer = new Rasterizer(gfx);
		vertexShader = new VertexShader(gfx);
		pixelShader = new PixelShader(gfx);
		inputLayout = new InputLayout(gfx, vertexShader->GetBlob());
	};

	void Update(Graphics& gfx)
	{
		float rotation[3] = { 0.0f, 0.0f, 0.0f };
		float scale[3] = { 1.0f, 1.0f, 1.0f };
		float translation[3] = { 0.0f, 0.0f, 10.0f };
		const ConstantBuffer::InstanceTransforms transforms = {
			XMMatrixTranspose(
				XMMatrixRotationRollPitchYaw(rotation[0], rotation[1], rotation[2]) *
				XMMatrixScaling(scale[0], scale[1], scale[2]) *
				XMMatrixTranslation(translation[0], translation[1], translation[2])
			),
			XMMatrixTranspose(XMMatrixIdentity()),
			XMMatrixTranspose(gfx.GetProjection())
		};
		instanceBuffer->Update(gfx, transforms);
	}

	void Bind(Graphics& gfx)
	{
		vertexBuffer->Bind(gfx);
		indexBuffer->Bind(gfx);
		inputLayout->Bind(gfx);
		vertexShader->Bind(gfx);
		pixelShader->Bind(gfx);
		instanceBuffer->Bind(gfx);
		topology->Bind(gfx);
	}

	void Draw(Graphics& gfx)
	{
		gfx.GetContext()->DrawIndexed(indices.size(), 0, 0);
	};
private:
	void LoadMesh(string path);
private:
	vector<Vertex> vertices;
	vector<unsigned short> indices;
private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	InputLayout* inputLayout;
	Topology* topology;
	PixelShader* pixelShader;
	VertexShader* vertexShader;
	ConstantBuffer* instanceBuffer;
	Rasterizer* rasterizer;
};

