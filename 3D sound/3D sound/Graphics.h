#pragma once

class Graphics {
public:
	Graphics(HWND hwnd);
	~Graphics();

	void Begin();
	void End();
	ID3D11Device* Device() { return m_device; }
	ID3D11DeviceContext* Context() { return m_context; }

	Graphics(Graphics& other) = delete;
	Graphics& operator=(Graphics& o) = delete;
private:
	IDXGISwapChain* m_swapchain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_context = nullptr;
	ID3D11RenderTargetView* m_backbuffer = nullptr;
	ID3D11DepthStencilView* m_depthstencil = nullptr;
};