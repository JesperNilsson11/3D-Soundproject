#pragma once

class Input {
public:
	static bool KeyDown(char c);
	static int XmouseDelta();
	static int YmouseDelta();
	static void SetMouse();
};