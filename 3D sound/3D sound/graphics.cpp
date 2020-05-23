#include "pch.h"
#include "Graphics.h"

Graphics::Graphics(HWND hwnd) {
	int width = 1920;
	int height = 1080;
	int multisample = 4;

	DXGI_SWAP_CHAIN_DESC swap_desc;
	ZeroMemory(&swap_desc, sizeof(swap_desc));
	swap_desc.BufferCount = 1;
	swap_desc.BufferDesc.Width = width;
	swap_desc.BufferDesc.Height = height;
	swap_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_desc.BufferDesc.RefreshRate.Numerator = 0;
	swap_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_desc.OutputWindow = hwnd;
	swap_desc.SampleDesc.Count = multisample;
	swap_desc.SampleDesc.Quality = 0;
	swap_desc.Windowed = FALSE;
	swap_desc.Flags = DXGI_SWAP_EFFECT_DISCARD;

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

	ID3D11Texture2D* pbackbuffer = nullptr;
	m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pbackbuffer);
	m_device->CreateRenderTargetView(pbackbuffer, NULL, &m_backbuffer);
	pbackbuffer->Release();

	D3D11_TEXTURE2D_DESC depth_desc;
	depth_desc.Width = swap_desc.BufferDesc.Width;
	depth_desc.Height = swap_desc.BufferDesc.Height;
	depth_desc.MipLevels = 1;
	depth_desc.ArraySize = 1;
	depth_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_desc.SampleDesc.Count = swap_desc.SampleDesc.Count;
	depth_desc.SampleDesc.Quality = swap_desc.SampleDesc.Quality;
	depth_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_desc.CPUAccessFlags = 0;
	depth_desc.MiscFlags = 0;

	ID3D11Texture2D* pdepthstencil;
	m_device->CreateTexture2D(&depth_desc, NULL, &pdepthstencil);


	D3D11_DEPTH_STENCIL_DESC dsDesc;

	// Depth test parameters
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Stencil test parameters
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = 0xFF;
	dsDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create depth stencil state
	ID3D11DepthStencilState* pDSState;
	m_device->CreateDepthStencilState(&dsDesc, &pDSState);
	m_context->OMSetDepthStencilState(pDSState, 1);
	pDSState->Release();

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	descDSV.Texture2D.MipSlice = 0;

	m_device->CreateDepthStencilView(pdepthstencil, &descDSV, &m_depthstencil);
	pdepthstencil->Release();

	m_context->OMSetRenderTargets(1, &m_backbuffer, m_depthstencil);

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = true; // changed to true
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = true; // changed to true
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	ID3D11RasterizerState* rs;
	m_device->CreateRasterizerState(&rasterDesc, &rs);
	m_context->RSSetState(rs);
	rs->Release();

	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	
	m_context->RSSetViewports(1, &vp);

	D3D11_BLEND_DESC b_desc;
	ZeroMemory(&b_desc, sizeof(b_desc));
	b_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	b_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	b_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	b_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	b_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	b_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	b_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	b_desc.AlphaToCoverageEnable = true;
	b_desc.IndependentBlendEnable = false;
	ID3D11BlendState* blendstate;
	m_device->CreateBlendState(&b_desc, &blendstate);
	float blendfact[4] = { 0 };
	UINT samplemask = 0xffffffff;
	m_context->OMSetBlendState(blendstate, blendfact, samplemask);
}

Graphics::~Graphics() {
	m_swapchain->SetFullscreenState(FALSE, NULL);

	m_swapchain->Release();
	m_backbuffer->Release();
	m_depthstencil->Release();
	m_device->Release();
	m_context->Release();
}

void Graphics::Begin() {
	static float color[4] = { 0.f, .0f, .0f, 1.f };

	m_context->ClearRenderTargetView(m_backbuffer, color);
	m_context->ClearDepthStencilView(m_depthstencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void Graphics::End() {
	//static float color[4] = { 0.f, .3f, .2f, 1.f };
	//color[0] += 0.00005f;
	//if (color[0] >= 1.f)
	//	color[0] = 0.f;

	//m_context->ClearRenderTargetView(m_backbuffer, color);

	m_swapchain->Present(0, 0);
}
