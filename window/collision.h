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

static bool intersectsXZ(const BoundingSphere& a, const BoundingSphere& b)
{
	float dx = a.centre.x - b.centre.x;
	float dz = a.centre.z - b.centre.z;
	float rr = a.radius + b.radius;
	return (dx * dx + dz * dz) < (rr * rr);
}

static bool sphereAABB_intersect(const BoundingSphere& s, const AABB& aabb) {
	float cx = std::max(aabb.min.x, std::min(s.centre.x, aabb.max.x));
	float cz = std::max(aabb.min.z, std::min(s.centre.z, aabb.max.z));

	float dx = s.centre.x - cx;
	float dz = s.centre.z - cz;

	return (dx * dx + dz * dz) < (s.radius * s.radius);
}

static bool raySphereIntersect(const Ray& r, const BoundingSphere& s, float& outT)
{
	Vec3 oc = r.o - s.centre;

	float a = Vec3::Dot(r.dir, r.dir);                
	float b = 2.0f * Vec3::Dot(oc, r.dir);
	float c = Vec3::Dot(oc, oc) - s.radius * s.radius;

	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f) return false;

	float sqrtDisc = sqrtf(disc);
	float t0 = (-b - sqrtDisc) / (2.0f * a);
	float t1 = (-b + sqrtDisc) / (2.0f * a);

	if (t0 > 0.0f) { outT = t0; return true; }
	if (t1 > 0.0f) { outT = t1; return true; }
	return false;
}
