#pragma once
#include "AnimationManager.h"
#include "Gun.h"
#include "Camera.h"

class Enemy {
public:
	AnimatedModel* model = nullptr;
	AnimationInstance anim;

	std::string name = "TRex";
	std::string currentAction;

	bool isAlive = true;
	int health = 200;
	bool dying = false;
	Vec3 pos;

	int damage = 25;
	float moveSpeed = 20.f;
	float chaseRange = 200.0f;
	float attackRange = 12.0f;

	float attackCooldown = 0.0f;
	float attackRate = 1.2f;
	float yaw = 0.f;

	void init(AnimatedModel* m, AnimationManager& animationManager, const Vec3& startPos) {
		model = m;
		pos = startPos;
		anim.init(&model->animation, 0);
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

	void draw(Core* core, Matrix& W, Matrix& vp) {
		if (!isAlive) return;
		model->draw(core, &anim, W, vp);
	}

	void update(float dt, AnimationManager& animationManager, Gun& gun, Camera& camera) {
		if (!isAlive) return;
		if (!currentAction.empty()) {
			anim.update(currentAction, dt);
		}
		if (dying && anim.animationFinished()) {
			isAlive = false;
			return;
		}
		if (attackCooldown > 0.0f) attackCooldown -= dt;

		Vec3 toPlayer = camera.pos - pos;
		toPlayer.y = 0.f;
		float dist2 = toPlayer.lengthSquare();
		float attack2 = attackRange * attackRange;
		float chase2 = chaseRange * chaseRange;
		if (dist2 <= attack2) {
			if (attackCooldown <= 0.0f) {
				play(animationManager, AnimationName::Attack, true, false);
				attackCooldown = attackRate;	
				gun.takeDamage(damage);
			}
			else {
				if (anim.animationFinished()) play(animationManager, AnimationName::Idle, false, true);
			}
			return;
		}

		if (dist2 <= chase2) {
			Vec3 dir = toPlayer.normalize();

			yaw = atan2f(dir.z, dir.x);

			pos += dir * moveSpeed * dt;

			play(animationManager, AnimationName::Run, false, true);
		}
		else {
			play(animationManager, AnimationName::Idle, false, true);
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

	void spawn(AnimatedModel* model, AnimationManager& animationManager, const Vec3& spawnPos) {
		Enemy e;
		e.init(model, animationManager, spawnPos);
		enemies.push_back(e);
	}

	void update(float dt, AnimationManager& animationManager, Gun& gun, Camera& camera) {
		for (auto& e : enemies) {
			e.update(dt, animationManager, gun, camera);
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
			Matrix W = Matrix::scale(Vec3(0.03, 0.03, 0.03)) * Matrix::rotateY(e.yaw + -M_PI/2) * Matrix::translate(e.pos);
			e.draw(core, W, vp);
		}
	}
};