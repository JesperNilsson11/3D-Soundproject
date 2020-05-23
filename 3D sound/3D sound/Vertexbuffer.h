#pragma once

#include "Graphics.h"

struct ID3D11Buffer;

struct Vertex {
	FLOAT x, y, z;
	FLOAT tex[2];
};

class VertexBuffer {
public:
	VertexBuffer(Graphics& g, Vertex* vertices, int num);

	void bind(Graphics& g);
private:
	ID3D11Buffer* m_buffer = nullptr;
};