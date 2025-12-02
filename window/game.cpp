#include "window.h"
#include "core.h"
#include "maths.h"
#include <fstream>
#include <sstream>
#include "pso.h"
#include "meshes.h"
#include "buffering.h"
#include "GamesEngineeringBase.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	Core core;
	win.init(1024, 1024, 0, 0, "My Window");
	core.init(win.hwnd, win.width, win.height);
	GamesEngineeringBase::Timer tim;

	Plane p;
	p.init(&core);

	const float SCALE_FACTOR = 0.1f;
	Vec3 scaleVec(SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR);
	Matrix scaleMatrix = Matrix().scale(scaleVec);

	Matrix planeWorld;
	float theta = 90.0f;    
	float fov = 1.0f;
	float nearZ = 0.1f;
	float farZ = 100.f;

	Matrix proj = Matrix().projMatrix((float)win.width, (float)win.height, theta, fov, farZ, nearZ);

	float time = 0.f;

	while (true) {
		float dt = tim.dt();
		time += dt;

		win.processMessages();
		if (win.keys[VK_ESCAPE] == 1) break;

		Vec3 from = Vec3(11 * cos(time), 5, 11 * sinf(time));
		Vec3 to = Vec3(0, 1, 0);
		Matrix v = Matrix().lookAt(to, from, Vec3(0, 1, 0));

		Matrix vp = v.mul(proj);

		Vec2 lights[4];
		lights[0] = Vec2(100.0f, 100.0f);
		lights[1] = Vec2(300.0f, 200.0f);
		lights[2] = Vec2(500.0f, 400.0f);
		lights[3] = Vec2(800.0f, 800.0f);

		core.beginFrame();
		p.draw(&core, planeWorld, vp, time, lights);
	
		core.finishFrame();
	}
	core.flushGraphicsQueue();
	return 0;
}