#pragma once

class Window {
public:
	Window(int width, int height, HINSTANCE instance);
	~Window();

	HWND getHandle() { return hwnd; }
private:
	HWND hwnd;
};