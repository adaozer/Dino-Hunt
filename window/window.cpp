#include "window.h"

Window* window;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			exit(0);
			return 0;
		}
		case WM_CLOSE: {
			PostQuitMessage(0);
			exit(0);
			return 0;
		}
		case WM_KEYDOWN: {
			window->keys[(unsigned int)wParam] = true;
			return 0;
		}
		case WM_KEYUP: {
			window->keys[(unsigned int)wParam] = false;
			return 0;
		}
		case WM_LBUTTONDOWN: {
			window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
			window->mouseButtons[0] = true;
			return 0;
		}
		case WM_LBUTTONUP: {
			window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
			window->mouseButtons[0] = false;
			return 0;
		}
		case WM_MOUSEMOVE: {
			window->updateMouse(WINDOW_GET_X_LPARAM(lParam), WINDOW_GET_Y_LPARAM(lParam));
			return 0;
		}
		default: {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}

void Window::init(float window_height, float window_width, float window_x, float window_y, std::string window_name) {
	WNDCLASSEX wc;
	hinstance = GetModuleHandle(NULL);
	name = window_name;
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	std::wstring wname = std::wstring(name.begin(), name.end());
	wc.lpszClassName = wname.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);
	RegisterClassEx(&wc);

	width = window_width;
	height = window_height;
	DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

	hwnd = CreateWindowEx(WS_EX_APPWINDOW, wname.c_str(), wname.c_str(), style,
		window_x, window_y, width, height, NULL, NULL, hinstance, this);

	window = this;
}


void Window::updateMouse(int x, int y) {
	if (firstMove) {
		mousex = x;
		mousey = y;
		dx = 0; dy = 0;
		firstMove = false;
		return;
	}
	dx = mousex - x;
	dy = mousey - y;
	mousex = x;
	mousey = y;
}

void Window::processMessages() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
