#include "pch.h"
#include "texture.h"
#include "lodepng.h"

Texture::Texture(Graphics& g) {
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = 256;
	desc.Height = 256;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	std::vector<char> image;
	for (int y = 0; y < 256; y++) {
		for (int x = 0; x < 256; x++) {
			char r = 0, g = 0, b = 0;
			if (x < 256 / 2 && y < 256 / 2) {
				r = 255;
			}
			else if (x >= 256 / 2 && y < 256 / 2) {
				g = 255;
			}
			else if (x >= 256 / 2 && y >= 256 / 2) {
				b = 255;
			}
			else {
				g = b = 255;
			}

			image.push_back(r);
			image.push_back(g);
			image.push_back(b);
			image.push_back(255);
		}
	}

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = image.data();
	data.SysMemPitch = 4 * 256;

	g.Device()->CreateTexture2D(&desc, &data, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(view_desc));
	view_desc.Format = desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MostDetailedMip = 0;
	view_desc.Texture2D.MipLevels = 1;

	g.Device()->CreateShaderResourceView(texture, &view_desc, &view);
}

Texture::Texture(Graphics& g, std::string file) {
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	file = assets + file;
	unsigned error = lodepng::decode(image, width, height, file);
	if (error) MessageBoxA(NULL, "png load failed", std::to_string(error).c_str(), 0);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_IMMUTABLE;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = image.data();
	data.SysMemPitch = 4 * width;

	g.Device()->CreateTexture2D(&desc, &data, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC view_desc;
	ZeroMemory(&view_desc, sizeof(view_desc));
	view_desc.Format = desc.Format;
	view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	view_desc.Texture2D.MostDetailedMip = 0;
	view_desc.Texture2D.MipLevels = 1;

	g.Device()->CreateShaderResourceView(texture, &view_desc, &view);
}

Texture::~Texture() {
	texture->Release();
}

void Texture::bind(Graphics& g, int slot) {
	g.Context()->PSSetShaderResources(slot, 1, &view);
}