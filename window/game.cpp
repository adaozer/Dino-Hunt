#include "window.h"
#include "core.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	Core core;
	win.init(1024, 1024, 0, 0, "My Window");
	core.init(win.hwnd, 1024, 1024);
	while (true) {
		core.beginFrame();
		win.processMessages();
		if (win.keys[VK_ESCAPE] == 1) break;
		core.finishFrame();
	}
	core.flushGraphicsQueue();
	return 0;
}