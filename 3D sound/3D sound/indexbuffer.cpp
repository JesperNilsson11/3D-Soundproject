#include "pch.h"
#include "IndexBuffer.h"

Indexbuffer::Indexbuffer(Graphics& g, int* indices, int num) {
    m_number = num;
    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(int) * num;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;

    iinitData.pSysMem = &indices[0];
    g.Device()->CreateBuffer(&indexBufferDesc, &iinitData, &m_buffer);
}

Indexbuffer::~Indexbuffer() {
    m_buffer->Release();
}

void Indexbuffer::bind(Graphics& g) {
    g.Context()->IASetIndexBuffer(m_buffer, DXGI_FORMAT_R32_UINT, 0);
}