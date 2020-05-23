#pragma once
#include <string>
#include <d3d11.h>
#include "Graphics.h"


class Cubemap {
public:
	Cubemap(Graphics& g, std::string file);
	~Cubemap();

	void Draw(Graphics& g);
private:
	ID3D11Buffer* sphereIndexBuffer = nullptr;
	ID3D11Buffer* sphereVertBuffer = nullptr;
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* view = nullptr;

	int numVertices = 0;
	int numFaces = 0;

public:
	struct Vertex {
		DirectX::XMFLOAT3 pos;
	};
};