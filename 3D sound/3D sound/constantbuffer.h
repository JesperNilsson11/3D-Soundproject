#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Graphics.h"

namespace dx = DirectX;

struct VSCB {
	DirectX::XMFLOAT4X4 matrix;
};

template <typename T>
class ConstantBuffer {
public:
	ConstantBuffer(Graphics& g, T t = {}) {
		D3D11_BUFFER_DESC desc;
		desc.ByteWidth = sizeof(T);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		T mat = {};
		dx::XMStoreFloat4x4(&mat.matrix, dx::XMMatrixTranslation(0.2, 0, 0));

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = &mat;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT result = g.Device()->CreateBuffer(&desc, &data, &m_buffer);

		if (FAILED(result))
			throw 3;
	}
	~ConstantBuffer() = default;

	void upload(Graphics& g, T& t) {
		D3D11_MAPPED_SUBRESOURCE subRes;
		ZeroMemory(&subRes, sizeof(subRes));

		HRESULT hr = g.Context()->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
		if (FAILED(hr)) {
			return;
		}
		memcpy(subRes.pData, &t, sizeof(T));
		g.Context()->Unmap(m_buffer, 0);
	}

	void bind(Graphics& g) {
		g.Context()->VSSetConstantBuffers(0, 1, &m_buffer);
	}
private:
	ID3D11Buffer* m_buffer = nullptr;
};

/*ConstantBuffer::ConstantBuffer(Graphics g) {
	D3D11_BUFFER_DESC desc;
	desc.ByteWidth = sizeof(VSCB);
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	VSCB mat = {};

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &mat.matrix;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	HRESULT result = 0;
}*/