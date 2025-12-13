#pragma once
#define NOMINMAX
#include <Windows.h>
#include <string>

#define WINDOW_GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define WINDOW_GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

class Window {
public:
	HWND hwnd;
	HINSTANCE hinstance;
	float width, height;
	std::string name;

	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];
	bool firstMove = true;

	int dx, dy;

	void init(float window_height, float window_width, float window_x, float window_y, std::string window_name);

	void updateMouse(int x, int y);

	void processMessages();

	void centerCursor()
	{
		POINT center;
		center.x = width / 2;
		center.y = height / 2;
		ClientToScreen(hwnd, &center);
		SetCursorPos(center.x, center.y);

		mousex = width / 2;
		mousey = height / 2;
	}
};