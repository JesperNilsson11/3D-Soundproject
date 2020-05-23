#pragma once

#include "Graphics.h"

struct ID3D11Buffer;

class Indexbuffer {
public:
	Indexbuffer(Graphics& g, int* indices, int num);
	~Indexbuffer();

	void bind(Graphics& g);
	int getNum() { return m_number; }
private:
	ID3D11Buffer* m_buffer = nullptr;
	int m_number = 0;
};