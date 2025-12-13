#pragma once
#include "AnimationManager.h"
class Enemy {
public:
	AnimatedModel* model = nullptr;
	AnimationInstance anim;

	std::string name = "TRex";
	std::string currentAction;

	bool isAlive = true;
	int health = 100;
	bool dying = false;

	Matrix world;

	void init(AnimatedModel* m, Matrix& W, AnimationManager& animationManager) {
		model = m;
		anim.init(&model->animation, 0);
		world = W;
		play(animationManager, AnimationName::Idle, true, true);
	}

	void takeDamage(int damage, AnimationManager& animationManager) {
		if (!isAlive) return;
		if (dying) return;
		health -= damage;
		if (health <= 0) {
			die(animationManager);
		}
	}

	void die(AnimationManager& animationManager) {
		if (dying) return;
		dying = true;
		play(animationManager, AnimationName::Death, true, false);
	}

	void draw(Core* core, Matrix& vp) {
		if (!isAlive) return;
		model->draw(core, &anim, world, vp);
	}

	void update(float dt, AnimationManager& animationManager) {
		if (!isAlive) return;
		if (!currentAction.empty()) {
			anim.update(currentAction, dt);
		}
		if (dying && anim.animationFinished()) {
			isAlive = false;
		}
	}

	void play(AnimationManager& animationManager, AnimationName action, bool restart, bool loop) {
		if (!animationManager.has(name, action)) return;

		const std::string& realAction = animationManager.get(name, action);
		if (!restart && realAction == currentAction) return;

		currentAction = realAction;
		anim.currentAnimation = currentAction;
		anim.looping = loop;

		if (restart) anim.resetAnimationTime();
	}
};

class EnemyManager {
public:
	std::vector<Enemy> enemies;

	void spawn(AnimatedModel* model, Matrix& W, AnimationManager& animationManager) {
		Enemy e;
		e.init(model, W, animationManager);
		enemies.push_back(e);
	}

	void update(float dt, AnimationManager& animationManager) {
		for (auto& e : enemies) {
			e.update(dt, animationManager);
		}

		for (size_t i = 0; i < enemies.size();) {
			if (!enemies[i].isAlive) {
				enemies.erase(enemies.begin() + i);
			}
			else {
				i++;
			}
		} 
	}

	void draw(Core* core, Matrix& vp) {
		for (auto& e : enemies) {
			e.draw(core, vp);
		}
	}
};