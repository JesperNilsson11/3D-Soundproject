#pragma once

#include "Graphics.h"

class ID3D11Buffer;

struct Vertex {
	FLOAT x, y, z;
	FLOAT color[4];
};

class VertexBuffer {
public:
	VertexBuffer(Graphics& g);

	void bind(Graphics& g);
private:
	ID3D11Buffer* m_buffer = nullptr;
};