#include "pch.h"
#include "shader.h"

VertexShader::VertexShader(LPCWSTR filename, Graphics& g) {
	//ID3D10Blob* VS;
	D3DReadFileToBlob(filename, &m_blob);

	g.Device()->CreateVertexShader(m_blob->GetBufferPointer(), m_blob->GetBufferSize(), NULL, &m_vs);
}

VertexShader::~VertexShader() {
	m_vs->Release();
}

void VertexShader::use(Graphics& g) {
	g.Context()->VSSetShader(m_vs, 0, 0);
}

PixelShader::PixelShader(LPCWSTR filename, Graphics& g) {
	ID3D10Blob* PS;
	D3DReadFileToBlob(filename, &PS);

	g.Device()->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_ps);
}

PixelShader::~PixelShader() {
	m_ps->Release();
}

void PixelShader::use(Graphics& g) {
	g.Context()->PSSetShader(m_ps, 0, 0);
}