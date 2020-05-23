#include "pch.h"
#include "cubemap.h"
#include "lodepng.h"

Cubemap::Cubemap(Graphics& g, std::string file) {
    int LatLines = 10;
    int LongLines = 10;

    numVertices = ((LatLines - 2) * LongLines) + 2;
    numFaces = ((LatLines - 3) * (LongLines) * 2) + (LongLines * 2);

    float sphereYaw = 0.0f;
    float spherePitch = 0.0f;

    std::vector<Vertex> vertices(numVertices);

    dx::XMVECTOR currVertPos = dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    vertices[0].pos.x = 0.0f;
    vertices[0].pos.y = 0.0f;
    vertices[0].pos.z = 1.0f;

    for (DWORD i = 0; i < LatLines - 2; ++i)
    {
        spherePitch = (i + 1) * (3.14 / (LatLines - 1));
        dx::XMMATRIX Rotationx = dx::XMMatrixRotationX(spherePitch);
        for (DWORD j = 0; j < LongLines; ++j)
        {
            sphereYaw = j * (6.28 / (LongLines));
            dx::XMMATRIX Rotationy = dx::XMMatrixRotationZ(sphereYaw);
            currVertPos = dx::XMVector3TransformNormal(dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), (Rotationx * Rotationy));
            currVertPos = dx::XMVector3Normalize(currVertPos);
            vertices[i * LongLines + j + 1].pos.x = dx::XMVectorGetX(currVertPos);
            vertices[i * LongLines + j + 1].pos.y = dx::XMVectorGetY(currVertPos);
            vertices[i * LongLines + j + 1].pos.z = dx::XMVectorGetZ(currVertPos);
        }
    }

    vertices[numVertices - 1].pos.x = 0.0f;
    vertices[numVertices - 1].pos.y = 0.0f;
    vertices[numVertices - 1].pos.z = -1.0f;


    D3D11_BUFFER_DESC vertexBufferDesc;
    ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * numVertices;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertexBufferData;

    ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
    vertexBufferData.pSysMem = &vertices[0];
    g.Device()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &sphereVertBuffer);


    std::vector<DWORD> indices(numFaces * 3);

    int k = 0;
    for (DWORD l = 0; l < LongLines - 1; ++l)
    {
        indices[k] = 0;
        indices[k + 1] = l + 1;
        indices[k + 2] = l + 2;
        k += 3;
    }

    indices[k] = 0;
    indices[k + 1] = LongLines;
    indices[k + 2] = 1;
    k += 3;

    for (DWORD i = 0; i < LatLines - 3; ++i)
    {
        for (DWORD j = 0; j < LongLines - 1; ++j)
        {
            indices[k] = i * LongLines + j + 1;
            indices[k + 1] = i * LongLines + j + 2;
            indices[k + 2] = (i + 1) * LongLines + j + 1;

            indices[k + 3] = (i + 1) * LongLines + j + 1;
            indices[k + 4] = i * LongLines + j + 2;
            indices[k + 5] = (i + 1) * LongLines + j + 2;

            k += 6; // next quad
        }

        indices[k] = (i * LongLines) + LongLines;
        indices[k + 1] = (i * LongLines) + 1;
        indices[k + 2] = ((i + 1) * LongLines) + LongLines;

        indices[k + 3] = ((i + 1) * LongLines) + LongLines;
        indices[k + 4] = (i * LongLines) + 1;
        indices[k + 5] = ((i + 1) * LongLines) + 1;

        k += 6;
    }

    for (DWORD l = 0; l < LongLines - 1; ++l)
    {
        indices[k] = numVertices - 1;
        indices[k + 1] = (numVertices - 1) - (l + 1);
        indices[k + 2] = (numVertices - 1) - (l + 2);
        k += 3;
    }

    indices[k] = numVertices - 1;
    indices[k + 1] = (numVertices - 1) - LongLines;
    indices[k + 2] = numVertices - 2;

    D3D11_BUFFER_DESC indexBufferDesc;
    ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(DWORD) * numFaces * 3;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;

    iinitData.pSysMem = &indices[0];
    g.Device()->CreateBuffer(&indexBufferDesc, &iinitData, &sphereIndexBuffer);

    std::vector<unsigned char> image; //the raw pixels
    unsigned width, height;

    //decode
    file = assets + file;
    unsigned error = lodepng::decode(image, width, height, file);
    if (error) MessageBoxA(NULL, "png load failed", std::to_string(error).c_str(), 0);

    width /= 4;
    height /= 3;

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 6;
    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

    int picmem = width * height * 4;
    int rowpitch = width * 4;
    int locations[6] = { 4 * picmem + rowpitch * 2, 4 * picmem, rowpitch, 8 * picmem + rowpitch, 4 * picmem + rowpitch, 4 * picmem + rowpitch * 3};
    
    D3D11_SUBRESOURCE_DATA data[6];
    ZeroMemory(&data, sizeof(data));
    for (int i = 0; i < 6; i++) {
        data[i].pSysMem = &image[locations[i]];
        data[i].SysMemPitch = 4 * width * 4;
        data[i].SysMemSlicePitch = 0;
    }

    g.Device()->CreateTexture2D(&desc, data, &texture);

    D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
    ZeroMemory(&view_desc, sizeof(view_desc));
    view_desc.Format = desc.Format;
    view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
    view_desc.TextureCube.MostDetailedMip = 0;
    view_desc.TextureCube.MipLevels = desc.MipLevels;

    g.Device()->CreateShaderResourceView(texture, &view_desc, &view);
}

Cubemap::~Cubemap() {
    sphereIndexBuffer->Release();
    sphereVertBuffer->Release();
    texture->Release();
    view->Release();
}

void Cubemap::Draw(Graphics& g) {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    g.Context()->IASetVertexBuffers(0, 1, &sphereVertBuffer, &stride, &offset);
    g.Context()->IASetIndexBuffer(sphereIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
    g.Context()->PSSetShaderResources(0, 1, &view);

    g.Context()->DrawIndexed(numFaces * 3, 0, 0);
}