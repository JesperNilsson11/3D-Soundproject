#pragma once

#include "Graphics.h"

class Texture {
public:
	Texture(Graphics& g);
	Texture(Graphics& g, std::string file);
	~Texture();

	void bind(Graphics& g, int slot = 0);

private:
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* view = nullptr;
};