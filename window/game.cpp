#include "window.h"
#include "core.h"
#include "maths.h"
#include <fstream>
#include <sstream>
#include "PSOManager.h"
#include "mesh.h"
#include "Plane.h"
#include "Cube.h"
#include "Sphere.h"
#include "ConstantBuffer.h"
#include "GamesEngineeringBase.h"
#include "GEMLoader.h"
#include "GEMObject.h"
#include "AnimatedModel.h"
#include "Camera.h"

void listAnimationNames(const GEMLoader::GEMAnimation& gemanimation)
{
	for (int i = 0; i < gemanimation.animations.size(); i++)
	{
		std::cout << gemanimation.animations[i].name << std::endl;
	}
}

void InitDebugConsole()
{
	AllocConsole();
	FILE* f;

	// Redirect stdout to the new console
	freopen_s(&f, "CONOUT$", "w", stdout);
	freopen_s(&f, "CONOUT$", "w", stderr);

	std::ios::sync_with_stdio(); // keep iostreams in sync with C stdio

	std::cout.clear();
	std::cerr.clear();

	std::cout << "Debug console initialised.\n";
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	Core core;
	win.init(1024, 1024, 0, 0, "My Window");
	core.init(win.hwnd, win.width, win.height);
	GamesEngineeringBase::Timer tim;
	ShaderManager shaderManager;
	InitDebugConsole();

	AnimatedModel am(&shaderManager);
	am.load(&core, "models/TRex.gem");

	GEMObject gem(&shaderManager);
	gem.init(&core, "models/water_tower_004.gem");

	AnimatedModel am1(&shaderManager);
	am1.load(&core, "models/Sheep-03.gem");

	AnimationInstance animatedInstance1;
	animatedInstance1.init(&am1.animation, 0);

	AnimationInstance animatedInstance;
	animatedInstance.init(&am.animation, 0);

	AnimatedModel am2(&shaderManager);
	am2.load(&core, "models/AutomaticCarbine.gem");

	listAnimationNames(am2.anim);

	AnimationInstance animatedInstance2;
	animatedInstance2.init(&am2.animation, 0);

	float fov = 90.0f;    
	float n = 0.01f;
	float f = 100000.f;
	float aspect = win.width / win.height;
	Matrix p = Matrix::projMatrix(aspect, fov, f, n);

	Camera cam;
	float time = 0.f;

	while (true) {
		core.beginFrame();

		float dt = tim.dt();
		win.processMessages();

		if (win.keys[VK_ESCAPE] == 1) break;

		time += dt;

		if (win.mouseButtons[0]) {
			cam.yaw += win.dx * cam.sensitivity;
			cam.pitch += win.dy * cam.sensitivity;

			float boundary = (89.f * M_PI) / 180.f;
			if (cam.pitch > boundary) cam.pitch = boundary;
			if (cam.pitch < -boundary) cam.pitch = -boundary;

			win.dx = 0;
			win.dy = 0;
		}

		Vec3 front = cam.front();
		Vec3 right = cam.right();

		if (win.keys['W']) cam.pos += front * cam.moveSpeed * dt;
		if (win.keys['A']) cam.pos -= right * cam.moveSpeed * dt;
		if (win.keys['S']) cam.pos -= front * cam.moveSpeed * dt;
		if (win.keys['D']) cam.pos += right * cam.moveSpeed * dt;

		Matrix v = cam.viewMatrix();
		Matrix vp = v * p;
		Matrix W;

		core.beginRenderPass();

		animatedInstance.update("run", dt);
		animatedInstance1.update("death", dt);
		animatedInstance2.update("18 empty reload", dt);

		if (animatedInstance1.animationFinished() == true) animatedInstance1.resetAnimationTime();
		if (animatedInstance.animationFinished() == true) animatedInstance.resetAnimationTime();
		if (animatedInstance2.animationFinished() == true) animatedInstance.resetAnimationTime();

		W = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f));
		am.draw(&core, &animatedInstance, W, vp);

		W = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f)) * Matrix::translate(Vec3(5, 0, 0));
		am1.draw(&core, &animatedInstance1, W, vp);

		W = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f)) * Matrix::translate(Vec3(10, 0, 0));
		gem.draw(&core, W, vp);

		W = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f)) * Matrix::translate(Vec3(15, 0, 0));
		am2.draw(&core, &animatedInstance2, W, vp);

		core.finishFrame();
	}
	core.flushGraphicsQueue();
	return 0;
}
