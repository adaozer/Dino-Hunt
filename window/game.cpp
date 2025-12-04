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
	/*
	float theta = 90.0f;    
	float fov = 1.0f;
	float nearZ = 0.01f;
	float farZ = 100000.f;

	Matrix proj = Matrix().projMatrix((float)win.width, (float)win.height, theta, fov, farZ, nearZ); */

	float time = 0.f;

	while (true) {
		core.beginFrame();

		float dt = tim.dt();
		win.processMessages();

		if (win.keys[VK_ESCAPE] == 1) break;

		time += dt;

		/* Vec3 from = Vec3(11 * cos(time), 5, 11 * sinf(time));
		Vec3 to = Vec3(0, 0, 0);
		Matrix v = Matrix().lookAt(to, from, Vec3(0, 1, 0));

		Matrix VP = proj.mul(v);
		Matrix W;
		Vec3 scaler(0.01f, 0.01f, 0.01f);
		W = W.scale(scaler); */
		
		Matrix vp;
		Matrix p = Matrix::perspective(0.01f, 10000.0f, 1920.0f / 1080.0f, 60.0f);
		Vec3 from = Vec3(11 * cos(time), 5, 11 * sinf(time));
		Matrix v = Matrix::lookAt(from, Vec3(0, 0, 0), Vec3(0, 1, 0));
		vp = v * p;
		Matrix W;

		core.beginRenderPass();

		animatedInstance.update("run", dt);
		if (animatedInstance.animationFinished() == true)
		{
			animatedInstance.resetAnimationTime();
		}
		shaderManager.updateConstantVS("vertexshader.hlsl", "staticMeshBuffer", "VP", &vp);
		W = Matrix::scaling(Vec3(0.01f, 0.01f, 0.01f));
		am.draw(&core, &animatedInstance, vp, W);
		core.finishFrame();
	}
	core.flushGraphicsQueue();
	return 0;
}
