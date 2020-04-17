#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd) {
	int width = 1080;
	int height = 720;

	DXGI_SWAP_CHAIN_DESC swap_desc;
	ZeroMemory(&swap_desc, sizeof(swap_desc));
	swap_desc.BufferCount = 1;
	swap_desc.BufferDesc.Width = width;
	swap_desc.BufferDesc.Height = height;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.OutputWindow = hwnd;
	swap_desc.SampleDesc.Count = 1;
	swap_desc.SampleDesc.Quality = 0;
	swap_desc.Windowed = TRUE;
	swap_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swap_desc,
		&m_swapchain,
		&m_device,
		NULL,
		&m_context);

	if (result != S_OK)
		throw 2;

	ID3D11Texture2D* pbackbuffer;
	m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbackbuffer);
	m_device->CreateRenderTargetView(pbackbuffer, NULL, &m_backbuffer);
	pbackbuffer->Release();

	m_context->OMSetRenderTargets(1, &m_backbuffer, NULL);

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = width;
	vp.Height = height;
	m_context->RSSetViewports(1, &vp);
}

Graphics::~Graphics() {
	m_swapchain->SetFullscreenState(FALSE, NULL);

	m_swapchain->Release();
	m_backbuffer->Release();
	m_device->Release();
	m_context->Release();
}

void Graphics::Render() {
	//static float color[4] = { 0.f, .3f, .2f, 1.f };
	//color[0] += 0.00005f;
	//if (color[0] >= 1.f)
	//	color[0] = 0.f;

	//m_context->ClearRenderTargetView(m_backbuffer, color);

	m_swapchain->Present(0, 0);
}
