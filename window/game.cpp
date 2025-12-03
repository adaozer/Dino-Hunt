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

	float theta = 90.0f;    
	float fov = 1.0f;
	float nearZ = 0.01f;
	float farZ = 100000.f;

	Matrix proj = Matrix().projMatrix((float)win.width, (float)win.height, theta, fov, farZ, nearZ);

	float time = 0.f;

	while (true) {
		float dt = tim.dt();
		time += dt;

		win.processMessages();
		if (win.keys[VK_ESCAPE] == 1) break;

		Vec3 from = Vec3(11 * cos(time), 5, 11 * sinf(time));
		Vec3 to = Vec3(0, 0, 0);
		Matrix v = Matrix().lookAt(to, from, Vec3(0, 1, 0));

		Matrix VP = proj.mul(v);
		Matrix W;
		Vec3 scaler(0.01f, 0.01f, 0.01f);
		W = W.scale(scaler);
		
		core.beginFrame();
		core.beginRenderPass();
		animatedInstance.update("run", dt);
		if (animatedInstance.animationFinished() == true)
		{
			animatedInstance.resetAnimationTime();
		}
		shaderManager.updateConstantVS("AnimatedUntextured", "staticMeshBuffer", "VP", &VP);
		am.draw(&core, &animatedInstance, VP, W);
		core.finishFrame();
	}
	core.flushGraphicsQueue();
	return 0;
}
