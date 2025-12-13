#pragma once
#include "AnimationManager.h"

class Gun {
public:
	AnimatedModel* model = nullptr;
	AnimationInstance anim;

	std::string name = "AutomaticCarbine";
	std::string actionName;

	int bullets = 50;
	int bulletCount = 50;

	bool reloading = false;
	float reloadTime = 0.f;
	float reloadLength = 1.875f;

	bool triggerPressed = false;
	bool triggerPressedThisFrame = false;
	float fireRate = 5.f;
	float shotCooldown = 0.f;

	void onTriggerPressed() { triggerPressed = true;  triggerPressedThisFrame = true; }
	void onTriggerReleased() { triggerPressed = false; }

	void init(AnimatedModel* m, AnimationManager& animationManager) {
		model = m;
		anim.init(&model->animation, 0);
		play(animationManager, AnimationName::Idle, true, true);
	}

	void reload(AnimationManager& animationManager) {
		if (reloading) return;
		if (bullets == bulletCount) return;
		reloading = true;
		reloadTime = reloadLength;
		play(animationManager, AnimationName::Reload, true, false);
	}

	void inspect(AnimationManager& animationManager) {
		if (reloading) return;
		if (triggerPressed) return;
		play(animationManager, AnimationName::Inspect, true, false);
	}

	void draw(Core* core, Matrix& W, Matrix& vp) {
		if (!model) return;
		model->draw(core, &anim, W, vp);
	}

	void update(float dt, AnimationManager& animationManager) {
		if (!actionName.empty())
			anim.update(actionName, dt);

		if (shotCooldown > 0.f)
			shotCooldown -= dt;

		if (reloading)
		{
			reloadTime -= dt;
			if (reloadTime <= 0.f)
			{
				reloading = false;
				bullets = bulletCount;
				reloadTime = 0.f;

				play(animationManager, AnimationName::Idle, false, true);
			}

			triggerPressedThisFrame = false;
			return;
		}

		if (triggerPressedThisFrame ||
			(triggerPressed && shotCooldown <= 0.f))
		{
			shoot(animationManager);
			triggerPressedThisFrame = false;
			return;
		}

		if (anim.animationFinished())
		{
			play(animationManager, AnimationName::Idle, false, true);
		}

		triggerPressedThisFrame = false;
	}

	void shoot(AnimationManager& animationManager) {
		if (bullets <= 0) {
			reload(animationManager);
			return;
		}
		bullets--;
		shotCooldown = 1.f / fireRate;
		play(animationManager, AnimationName::Attack, true, false);
	}

	void play(AnimationManager& animationManager, AnimationName action, bool restart, bool loop) {
		if (!animationManager.has(name, action)) return;

		const std::string& realAction = animationManager.get(name, action);
		if (!restart && realAction == actionName) return;

		actionName = realAction;
		anim.currentAnimation = actionName;
		anim.looping = loop;

		if (restart) anim.resetAnimationTime();
	}
};