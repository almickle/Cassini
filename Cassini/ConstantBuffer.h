#pragma once
#include "Graphics.h"

class ConstantBuffer
{
public:
	ConstantBuffer( Graphics& gfx )
	{
		D3D11_BUFFER_DESC cbDesc = { };
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.ByteWidth = sizeof( InstanceTransforms ); // Size of the constant buffer structure
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		gfx.GetDevice()->CreateBuffer( &cbDesc, nullptr, pConstBuffer.GetAddressOf() );
	}

	struct InstanceTransforms
	{
		XMMATRIX worldMatrix; // World transformation matrix
		XMMATRIX viewMatrix; // View transformation matrix
		XMMATRIX projectionMatrix; // Projection transformation matrix
	};

	void Bind( Graphics& gfx )
	{
		gfx.GetContext()->VSSetConstantBuffers( 0, 1, pConstBuffer.GetAddressOf() );
		bound = true;
	}

	void Update( Graphics& gfx, const InstanceTransforms& cbData )
	{
		if (bound == false)
		{
			return;
		};
		// Map the constant buffer
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		gfx.GetContext()->Map( pConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

		// Copy data to the constant buffer
		memcpy( mappedResource.pData, &cbData, sizeof( InstanceTransforms ) );

		// Unmap the constant buffer
		gfx.GetContext()->Unmap( pConstBuffer.Get(), 0 );
	}

private:
	ComPtr<ID3D11Buffer> pConstBuffer;
	bool bound = false;
};
