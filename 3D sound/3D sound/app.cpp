#include "pch.h"
#include "app.h"
#include "input.h"
#include <random>

float Distance(dx::XMFLOAT3 p1, dx::XMFLOAT3 p2);

Vertex OurVertices[] =
{
	{0.0f, 0.5f, 0.0f, 0.5f, 1.0f},
	{0.45f, -0.5, 0.0f, 1.0f, 0.0f},
	{-0.45f, -0.5f, 0.0f, 0.0f, 0.0f}
};

Vertex Terrain[] =
{
    {-100.0f, -1.0f, -100.0f, 0.0f, 0.0f},
    {-100.0f, -1.0f, 100.0f, 0.0f, 1.0f},
    {100.0f, -1.0f, 100.0f, 1.0f, 1.0f},
    {-100.0f, -1.0f, -100.0f, 0.0f, 0.0f},
    {100.0f, -1.0f, 100.0f, 1.0f, 1.0f},
    {100.0f, -1.0f, -100.0f, 1.0f, 0.0f}
    //{-10.0f, -1.0f, -10.0f, 1.0f, 1.0f, 1.0f, 1.0f},
    //{-10.0f, -1.0f, 10.0f, 1.0f, 1.0f, 1.0f, 1.0f},
    //{10.0f, -1.0f, 10.0f, 1.0f, 1.0f, 1.0f, 1.0f},
    //{-10.0f, -1.0f, -10.0f, 1.0f, 1.0f, 1.0f, 1.0f},
    //{10.0f, -1.0f, 10.0f, 1.0f, 1.0f, 1.0f, 1.0f},
    //{10.0f, -1.0f, -10.0f, 1.0f, 1.0f, 1.0f, 1.0f}
};

App::App(HINSTANCE hintstance) : m_window(1920, 1080, hintstance), m_graphics(m_window.getHandle()),
m_vs(L"vertex.cso", m_graphics), m_ps(L"pixel.cso", m_graphics), m_vb(m_graphics, OurVertices, 3), m_terrain(m_graphics, Terrain, 6),m_cb(m_graphics), m_texture(m_graphics), m_texture_terrain(m_graphics, "GrassTerrain.png"),
m_cubemap_vs(L"cubemapVS.cso", m_graphics), m_cubemap_ps(L"cubemapPS.cso", m_graphics), m_cubemap(m_graphics, "sky.png"),
//m_gun(m_graphics, "Handgun_fbx_7.4_binary.fbx"),
m_dog(m_graphics, "13463_Australian_Cattle_Dog_v3.obj"), m_dog_texture(m_graphics, "Australian_Cattle_Dog_dif.png"),
m_tree_bark(m_graphics, "MapleTreeStem.obj"), m_texture_tree(m_graphics, "maple_bark.png"),
m_leaves(m_graphics, "MapleTreeLeaves.obj"), m_texture_leaves(m_graphics, "maple_leaf.png"),
m_guitar(m_graphics, "Gibson 335/Gibson 335_High_Poly.obj"), m_guitar_texture(m_graphics, "Gibson 335/Texturas/Tex_Caja_2.png"),
m_church(m_graphics, "church/10063_Church_v3.obj"), m_church_texture(m_graphics, "church/church.png")  {
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    D3D11_INPUT_ELEMENT_DESC ied_sky[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    auto VS = m_vs.getRaw();
    m_graphics.Device()->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_layout);
    m_graphics.Context()->IASetInputLayout(m_layout);

    VS = m_cubemap_vs.getRaw();
    m_graphics.Device()->CreateInputLayout(ied_sky, 1, VS->GetBufferPointer(), VS->GetBufferSize(), &m_layout_cubemap);

    D3D11_SAMPLER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.MaxAnisotropy = 16;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    m_graphics.Device()->CreateSamplerState(&desc, &m_sampler);

    dx::XMMATRIX matrix = dx::XMMatrixScaling(0.01f, 0.01f, 0.01f);
    dx::XMVECTOR quat = dx::XMQuaternionRotationRollPitchYaw(-dx::XM_PIDIV2, 0.f, 0.f);
    matrix = dx::XMMatrixAffineTransformation({ 0.05f, 0.05f, 0.05f }, { 1.f, 0.f, 0.f }, quat, { 5.f,-1.f,0.f });
    dx::XMFLOAT4X4 mat;
    dx::XMStoreFloat4x4(&mat, matrix);
    m_dog.SetModelMatrix(mat);

    quat = dx::XMQuaternionRotationRollPitchYaw(0.f, 0.f, -dx::XM_PIDIV2);
    matrix = dx::XMMatrixAffineTransformation({ 0.1f, 0.1f, 0.1f }, { 1.f, 0.f, 0.f }, quat, { -15.f, 0.f,0.f });
    dx::XMStoreFloat4x4(&mat, matrix);
    m_guitar.SetModelMatrix(mat);

    quat = dx::XMQuaternionRotationRollPitchYaw(-dx::XM_PIDIV2, dx::XM_PI, 0.f);
    matrix = dx::XMMatrixAffineTransformation({ 0.01f, 0.01f, 0.01f }, { 1.f, 0.f, 0.f }, quat, { -5.f, -1.f, 20.f });
    dx::XMStoreFloat4x4(&mat, matrix);
    m_church.SetModelMatrix(mat);

    m_amb = m_sounssystem.createSound("ambulanc.wav");
    m_sound = m_sounssystem.createSound("cello.wav");
    m_barking = m_sounssystem.createSound("barking.wav");
    m_guitar_sound = m_sounssystem.createSound("guitar.wav");
    m_bell = m_sounssystem.createSound("bell.wav");
    m_sounssystem.createBackgroundSound("birds.wav");
    m_sounssystem.setPosition(m_barking, { 5.f, 0.f,0.f });
    m_sounssystem.setPosition(m_guitar_sound, { -15.f, 1.f,0.f });
    m_sounssystem.setPosition(m_bell, { -5.f, 5.f, 13.f });
    m_sounssystem.play(m_barking);
}

void App::Run() {
    MSG msg = { };
    
    m_sounssystem.start();
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    // Game Loop
    while (m_running) {
        Input::SetMouse();
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                m_running = false;
        }
        
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;
        auto dt = duration.count();
        start = end;

        m_camera.update(dt);

        // SOUND LOGIC
        dx::XMFLOAT3 temp = m_camera.Direction();
        m_sounssystem.update(dt, m_camera.Position(), { temp.x, temp.z });

        m_sounssystem.play(m_barking);
        if (Input::KeyDown('G')) {
            m_sounssystem.play(m_guitar_sound);
        }
        if (Input::KeyDown('B')) {
            m_sounssystem.play(m_bell);
        }

        static int counter = 0;
        counter++;
        static double time = 0;
        time += dt;
        //if (counter >= 1000) {
        //    SetWindowTextA(m_window.hwnd, (std::to_string(static_cast<int>(1000 / time)) + " fps ").c_str());
        //    time = 0;
        //    counter = 0;
        //}
        VSCB mat;
        m_graphics.Begin();
        m_graphics.Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_graphics.Context()->PSSetSamplers(0, 1, &m_sampler);
        
        mat.matrix = m_camera.CubeVP();
        m_cubemap_vs.use(m_graphics);
        m_cubemap_ps.use(m_graphics);
        m_cb.bind(m_graphics);
        m_cb.upload(m_graphics, mat);
        m_graphics.Context()->IASetInputLayout(m_layout_cubemap);
        m_cubemap.Draw(m_graphics);

        m_vs.use(m_graphics);
        m_ps.use(m_graphics);
        m_graphics.Context()->IASetInputLayout(m_layout);
        m_vb.bind(m_graphics);
        m_texture.bind(m_graphics);
        dx::XMFLOAT4X4 vp_float = m_camera.ViewProjection();
        dx::XMFLOAT4X4 model_float;
        dx::XMMATRIX vp = dx::XMLoadFloat4x4(&vp_float);
        dx::XMMATRIX model_matrix;
        mat.matrix = vp_float;
        m_cb.upload(m_graphics, mat);

        //m_graphics.Context()->Draw(3, 0);

        m_terrain.bind(m_graphics);
        m_texture_terrain.bind(m_graphics);
        m_graphics.Context()->Draw(6, 0);

        //m_gun.Draw(m_graphics);
        model_float = m_dog.GetModelMatrix();
        model_matrix = dx::XMLoadFloat4x4(&model_float);
        model_matrix = dx::XMMatrixMultiply(model_matrix, vp);
        dx::XMStoreFloat4x4(&mat.matrix, model_matrix);
        m_cb.upload(m_graphics, mat);
        m_dog_texture.bind(m_graphics);
        m_dog.Draw(m_graphics);

        model_float = m_guitar.GetModelMatrix();
        model_matrix = dx::XMLoadFloat4x4(&model_float);
        model_matrix = dx::XMMatrixMultiply(model_matrix, vp);
        dx::XMStoreFloat4x4(&mat.matrix, model_matrix);
        m_cb.upload(m_graphics, mat);
        m_guitar_texture.bind(m_graphics);
        m_guitar.Draw(m_graphics);

        model_float = m_church.GetModelMatrix();
        model_matrix = dx::XMLoadFloat4x4(&model_float);
        model_matrix = dx::XMMatrixMultiply(model_matrix, vp);
        dx::XMStoreFloat4x4(&mat.matrix, model_matrix);
        m_cb.upload(m_graphics, mat);
        m_church_texture.bind(m_graphics);
        m_church.Draw(m_graphics);

        // Render trees
        std::default_random_engine gen;
        for (int i = 0; i < 100; i++) {
            std::uniform_int_distribution<int> dist(-90, 90);
            dx::XMVECTOR quat = dx::XMQuaternionRotationRollPitchYaw(0.f, 0.f, 0.f);
            model_matrix = dx::XMMatrixAffineTransformation({ 0.1f, 0.1f, 0.1f }, { 1.f, 0.f, 0.f }, quat, { (float)dist(gen),-1.f,(float)dist(gen) });;
            model_matrix = dx::XMMatrixMultiply(model_matrix, vp);
            dx::XMStoreFloat4x4(&mat.matrix, model_matrix);
            m_cb.upload(m_graphics, mat);
            m_texture_tree.bind(m_graphics);
            m_tree_bark.Draw(m_graphics);

            m_texture_leaves.bind(m_graphics);
            m_leaves.Draw(m_graphics);
        }

        m_graphics.End();
    }
}