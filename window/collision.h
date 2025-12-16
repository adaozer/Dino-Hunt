#pragma once
#include "maths.h"

class BoundingSphere
{
public:
	Vec3 centre;
	float radius;
};

class Ray
{
public:
	Vec3 o;
	Vec3 dir;
	Vec3 invdir;
	Ray() {}
	Ray(const Vec3 _o, const Vec3 _dir) {
		init(_o, _dir);
	}
	void init(const Vec3 _o, const Vec3 _dir) {
		o = _o;
		dir = _dir;
		invdir = Vec3(1.0f, 1.0f, 1.0f) / dir;
	}
	Vec3 at(const float t) {
		return (o + (dir * t));
	}
};

class AABB
{
public:
	Vec3 max;
	Vec3 min;
	AABB() { reset(); }
	void reset()
	{
		max = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
	}
	void extend(const Vec3& p)
	{
		max = Max(max, p);
		min = Min(min, p);
	}

	bool rayAABB(const Ray& r, float& t)
	{
		Vec3 s = (min - r.o) * r.invdir;
		Vec3 l = (max - r.o) * r.invdir;
		Vec3 s1 = Min(s, l);
		Vec3 l1 = Max(s, l);
		float ts = std::max(s1.x, std::max(s1.y, s1.z));
		float tl = std::min(l1.x, std::min(l1.y, l1.z));
		t = std::min(ts, tl);
		return (ts < tl);
	}
};
