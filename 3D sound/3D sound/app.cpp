#include "pch.h"
#include "app.h"

App::App(HINSTANCE hintstance) : m_window(1080, 720, hintstance), m_graphics(m_window.getHandle()),
m_vs(L"vertex.cso", m_graphics), m_ps(L"pixel.cso", m_graphics), m_vb(m_graphics) {
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    auto VS = m_vs.getRaw();
    m_graphics.Device()->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_layout);
    m_graphics.Context()->IASetInputLayout(m_layout);
}

void App::Run() {
    MSG msg = { };
    while (m_running) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                m_running = false;
        }
        m_vs.use(m_graphics);
        m_ps.use(m_graphics);
        m_vb.bind(m_graphics);
        m_graphics.Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        m_graphics.Context()->Draw(3, 0);
        m_graphics.Render();
    }
}