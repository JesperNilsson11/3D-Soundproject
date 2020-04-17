#pragma once

#include "window.h"
#include "Graphics.h"
#include "shader.h"
#include "Vertexbuffer.h"

class App {
public:
	App(HINSTANCE hintstance);
	void Run();
private:
	Window m_window;
	Graphics m_graphics;
	VertexShader m_vs;
	PixelShader m_ps;
	ID3D11InputLayout* m_layout = nullptr;
	VertexBuffer m_vb;
	bool m_running = true;
};