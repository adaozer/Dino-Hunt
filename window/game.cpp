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

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	Core core;
	win.init(1024, 1024, 0, 0, "My Window");
	core.init(win.hwnd, win.width, win.height);
	GamesEngineeringBase::Timer tim;
	ShaderManager shaderManager;

	AnimatedModel am(&shaderManager);
	am.load(&core, "TRex.gem");

	AnimationInstance animatedInstance;
	animatedInstance.init(&am.animation, 0);

	float fov = 90.0f;    
	float n = 0.01f;
	float f = 100000.f;
	float aspect = win.width / win.height;

	float time = 0.f;

	while (true) {
		core.beginFrame();

		float dt = tim.dt();
		win.processMessages();

		if (win.keys[VK_ESCAPE] == 1) break;

		time += dt;
		
		Matrix p = Matrix::projMatrix(aspect, fov, f, n);
		Vec3 from = Vec3(11 * cos(time), 5, 11 * sinf(time));
		Matrix v = Matrix::lookAt(Vec3(0, 0, 0), from, Vec3(0, 1, 0));
		Matrix vp = v * p;
		Matrix W;
		W = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f));

		core.beginRenderPass();

		animatedInstance.update("run", dt);
		if (animatedInstance.animationFinished() == true)
		{
			animatedInstance.resetAnimationTime();
		}

		am.draw(&core, &animatedInstance, vp, W);
		core.finishFrame();
	}
	core.flushGraphicsQueue();
	return 0;
}
