// 3D sound.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "app.h"
#include "input.h"
#include <windowsx.h>

static std::unordered_map<WPARAM, bool> keys;
//static int xPos = 500;
//static int yPos = 500;
//static int lastXPos = 500;
//static int lastYPos = 500;
static int xdelta = 0;
static int ydelta = 0;
static bool showcursor = false;

void Input::SetMouse() {
    if (!showcursor) {
        //xdelta = xPos - lastXPos;
        //SetCursorPos(500, 500);
        //xPos = 500;
        //lastXPos = 500;
        RECT r;
        r.left = 400;
        r.top = 400;
        r.right = 420;
        r.bottom = 420;
        ClipCursor(&r);
    }
    else {
        ClipCursor(nullptr);
    }
    //lastXPos = xPos;
    xdelta = 0;
    ydelta = 0;
}

bool Input::KeyDown(char c) {
    return keys[c];
}

int Input::XmouseDelta() {
    return xdelta;
}

int Input::YmouseDelta() {
    return ydelta;
}

//LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nShowCmd) {
    
    App app(hInstance);
    
    app.Run();

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        //SetWindowTextA(hwnd, (std::to_string(xPos) + " " + std::to_string(lastXPos)).c_str());
        keys[wParam] = true;
        return 0;
    case WM_KEYUP:
        keys[wParam] = false;
        if (wParam == VK_ESCAPE) {
            showcursor = !showcursor;
            ShowCursor(showcursor);
        }
        return 0;
    //case WM_MOUSEMOVE: {
    //    if (!showcursor) {
    //        lastXPos = xPos;
    //        lastYPos = yPos;
    //        xPos = GET_X_LPARAM(lParam);
    //        yPos = GET_Y_LPARAM(lParam);
    //        //SetWindowTextA(hwnd, (std::to_string(xPos) + " " + std::to_string(yPos)).c_str());
    //        //SetCursorPos(500, 500);
    //        //lastXPos = 500;
    //        //lastYPos = 500;
    //    }
    //    return 0;
    //}
    case WM_INPUT:
    {
        if (!showcursor) {
            UINT dwSize = 48;
            static BYTE lpb[48];

            GetRawInputData((HRAWINPUT)lParam, RID_INPUT,
                lpb, &dwSize, sizeof(RAWINPUTHEADER));

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                int xPosRelative = raw->data.mouse.lLastX;
                int yPosRelative = raw->data.mouse.lLastY;
                xdelta += xPosRelative;
                ydelta += yPosRelative;
            }
        }
        break;
    }
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
