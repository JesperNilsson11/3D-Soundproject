#pragma once

#include "Graphics.h"
struct ID3D10Blob;

class VertexShader {
public:
	VertexShader(LPCWSTR filename, Graphics& g);
	~VertexShader();
	void use(Graphics& g);
	ID3D10Blob* getRaw() { return m_blob; }
private:
	ID3D11VertexShader* m_vs = nullptr;
	ID3D10Blob* m_blob = nullptr;
};

class PixelShader {
public:
	PixelShader(LPCWSTR filename, Graphics& g);
	~PixelShader();
	void use(Graphics& g);
private:
	ID3D11PixelShader* m_ps = nullptr;
};