#pragma once

#include "window.h"
#include "Graphics.h"
#include "shader.h"
#include "Vertexbuffer.h"
#include "constantbuffer.h"
#include "camera.h"
#include "texture.h"
#include "cubemap.h"
#include "model.h"
#include "soundsystem.h"

class App {
public:
	App(HINSTANCE hintstance);
	void Run();
private:
	Window m_window;
	Graphics m_graphics;
	VertexShader m_vs;
	PixelShader m_ps;
	VertexShader m_cubemap_vs;
	PixelShader m_cubemap_ps;
	Cubemap m_cubemap;
	ID3D11InputLayout* m_layout = nullptr;
	ID3D11InputLayout* m_layout_cubemap = nullptr;
	VertexBuffer m_vb;
	VertexBuffer m_terrain;
	ConstantBuffer<VSCB> m_cb;
	Texture m_texture;
	Texture m_texture_terrain;
	Camera m_camera;
	//Model m_gun;
	Model m_tree_bark;
	Texture m_texture_tree;
	Model m_leaves;
	Texture m_texture_leaves;
	Model m_dog;
	Texture m_dog_texture;
	Model m_guitar;
	Texture m_guitar_texture;
	Model m_church;
	Texture m_church_texture;
	SoundSystem m_sounssystem;
	Sound m_sound;
	Sound m_amb;
	Sound m_barking;
	Sound m_guitar_sound;
	Sound m_bell;
	ID3D11SamplerState* m_sampler = nullptr;
	bool m_running = true;
};