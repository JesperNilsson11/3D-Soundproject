#include "pch.h"
#include "Vertexbuffer.h"

VertexBuffer::VertexBuffer(Graphics& g, Vertex* vertices, int num) {
	//Vertex OurVertices[] =
	//{
	//	{0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f},
	//	{0.45f, -0.5, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f},
	//	{-0.45f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f}
	//};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(Vertex) * num;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	g.Device()->CreateBuffer(&bd, NULL, &m_buffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	g.Context()->Map(m_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, vertices, bd.ByteWidth);
	g.Context()->Unmap(m_buffer, NULL);
}

void VertexBuffer::bind(Graphics& g) {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	g.Context()->IASetVertexBuffers(0, 1, &m_buffer, &stride, &offset);
}