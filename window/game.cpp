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
#include "Texture.h"
#include "AnimationManager.h"
#include "Gun.h"
#include "Enemy.h"
#include "InstancedObject.h"

void listAnimationNames(AnimatedModel am)
{
	for (auto& [name, seq] : am.animation.animations) {
		std::cout << name << " : "
			<< seq.frames.size() << " frames, "
			<< seq.ticksPerSecond << " tps, "
			<< seq.duration() << " sec\n";
	}

}
//center cursor
// animasyon manageri tam anlamak
// play functionlari
// gun update
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
	Window win;
	Core core;
	win.init(1024, 1024, 0, 0, "My Window");
	core.init(win.hwnd, win.width, win.height);
	GamesEngineeringBase::Timer tim;
	ShaderManager shaderManager;
	TextureManager tex;

	/*
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	*/
	AnimatedModel gunModel(&shaderManager, &tex, "textures/AC5_Albedo_alb.png");
	gunModel.load(&core, "models/AutomaticCarbine.gem");

	AnimatedModel enemyModel(&shaderManager, &tex, "textures/T-rex_Base_Color_alb.png");
	enemyModel.load(&core, "models/TRex.gem");

	std::vector<INSTANCE> treeInstances;
	treeInstances.reserve(500);

	for (int i = 0; i < 500; i++) {
		float x = (float)((rand() % 400) - 200);
		float z = (float)((rand() % 400) - 200);

		INSTANCE inst;
		inst.W = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f)) * Matrix::translate(Vec3(x, 0.f, z));
		treeInstances.push_back(inst);
	}

	InstancedObject bamboo(&shaderManager, &tex, "textures/bamboo branch_ALB.png");
	bamboo.init(&core, "models/bamboo.gem", treeInstances);

	std::vector<INSTANCE> grassInstances;
	grassInstances.reserve(500);

	for (int i = 0; i < 500; i++) {
		float x = (float)((rand() % 400) - 200);
		float z = (float)((rand() % 400) - 200);

		INSTANCE inst;
		inst.W = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f)) * Matrix::translate(Vec3(x, 0.f, z));
		grassInstances.push_back(inst);
	}

	InstancedObject grass(&shaderManager, &tex, "textures/TX_GrassClumps_01_ALB.png");
	grass.init(&core, "models/Grass_Clump_01c.gem", grassInstances);

	AnimationManager animationManager;
	EnemyManager enemyManager;

	animationManager.set("AutomaticCarbine", AnimationName::Idle, "04 idle");
	animationManager.set("AutomaticCarbine", AnimationName::Attack, "08 fire");
	animationManager.set("AutomaticCarbine", AnimationName::Reload, "17 reload");
	animationManager.set("AutomaticCarbine", AnimationName::Inspect, "05 inspect");

	animationManager.set("TRex", AnimationName::Idle, "run");
	animationManager.set("TRex", AnimationName::Attack, "attack");

	Gun gun;
	gun.init(&gunModel, animationManager);

	Matrix enemyW = Matrix::scale(Vec3(0.01f, 0.01f, 0.01f)) * Matrix::translate(Vec3(10, 0, 0));
	enemyManager.spawn(&enemyModel, enemyW, animationManager);

	Plane plane(&shaderManager);
	plane.init(&core);

	Sphere sphere(&shaderManager, &tex, "textures/kloofendal_48d_partly_cloudy_puresky.jpg");
	sphere.init(&core);

	float fov = 90.0f;
	float n = 0.01f;
	float f = 100000.f;
	float aspect = win.width / win.height;
	Matrix p = Matrix::projMatrix(aspect, fov, f, n);
	Camera cam;
	float time = 0.f;

	bool lastMouseDown = false;
	bool centered = false;
	bool lastZPress = false;

	while (true) {
		core.beginFrame();

		float dt = tim.dt();
		win.processMessages();
		if (centered) {
			win.centerCursor();
		}

		if (win.keys[VK_ESCAPE] == 1) break;
		bool zPress = win.keys['Z'];
		if (zPress && !lastZPress) {
			if (centered) {
				centered = false;
				ShowCursor(TRUE);
			}
			else {
				centered = true;
				ShowCursor(FALSE);
			}
		}
		lastZPress = zPress;

		time += dt;
		bool mouseDown = win.mouseButtons[0];

		if (mouseDown && !lastMouseDown) gun.onTriggerPressed();
		if (!mouseDown && lastMouseDown) gun.onTriggerReleased();

		lastMouseDown = mouseDown;

		cam.yaw += win.dx * cam.sensitivity;
		cam.pitch += win.dy * cam.sensitivity;

		float boundary = (89.f * M_PI) / 180.f;
		if (cam.pitch > boundary) cam.pitch = boundary;
		if (cam.pitch < -boundary) cam.pitch = -boundary;

		win.dx = 0;
		win.dy = 0;

		float yaw = cam.yaw;

		Vec3 forward = Vec3(std::cos(yaw), 0.0f, std::sin(yaw));
		Vec3 right = Vec3(-std::sin(yaw), 0.0f, std::cos(yaw));

		Vec3 move(0.f, 0.f, 0.f);

		if (win.keys['W']) move += forward;
		if (win.keys['A']) move += right;
		if (win.keys['S']) move -= forward;
		if (win.keys['D']) move -= right;
		if (win.keys['R']) gun.reload(animationManager);
		if (win.keys['F']) gun.inspect(animationManager);

		if (move.lengthSquare() > 0.f) {
			move = move.normalize();
			cam.pos += move * cam.moveSpeed * dt;
		}
		cam.pos.y = 3.5;

		Matrix v = cam.viewMatrix();
		Matrix camWorld = v.invert();
		Vec3 offsetCam = Vec3(0, 0, 0);
		Matrix vp = v * p;
		Matrix local = Matrix::scale(Vec3(0.02f, 0.02f, 0.02f)) * Matrix::rotateY(M_PI) * Matrix::translate(offsetCam);

		Matrix gunW = local * camWorld;
		
		gun.update(dt, animationManager);
		enemyManager.update(dt, animationManager);

		core.beginRenderPass();

		gun.draw(&core, gunW, vp);
		enemyManager.draw(&core, vp);

		bamboo.draw(&core, vp);
		Matrix W;

		grass.draw(&core, vp);

		plane.draw(&core, W, vp);

		sphere.draw(&core, W, vp);

		core.finishFrame();
	}
	core.flushGraphicsQueue();
	return 0;
}