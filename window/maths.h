#pragma once
#define SQ(x) ((x) * (x))
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <numbers>
constexpr float M_PI = std::numbers::pi_v<float>;

template<typename T>
static T lerp(const T a, const T b, float t) {
	return a * (1.0f - t) + (b * t);
}

class Vec3 {
public:
	Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Vec3() : x(0), y(0), z(0) {}

	union {
		float v[3];
		struct { float x, y, z; };
	};

	Vec3& operator+=(const Vec3& pVec) {
		v[0] += pVec.v[0];
		v[1] += pVec.v[1];
		v[2] += pVec.v[2];
		return *this;
	}

	Vec3 operator+(const Vec3& pVec) const {
		return Vec3(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2]);
	}

	Vec3 operator+(const float val) const {
		return Vec3(v[0] + val, v[1] + val, v[2] + val);
	}

	Vec3& operator*=(const Vec3& pVec) {
		v[0] *= pVec.v[0];
		v[1] *= pVec.v[1];
		v[2] *= pVec.v[2];
		return *this;
	}

	Vec3 operator*(const float val) const {
		return Vec3(v[0] * val, v[1] * val, v[2] * val);
	}

	Vec3 operator*(const Vec3& v2) const {
		return Vec3(v[0] * v2.x, v[1] * v2.y, v[2] * v2.z);
	}

	Vec3& operator-=(const Vec3& pVec) {
		v[0] -= pVec.v[0];
		v[1] -= pVec.v[1];
		v[2] -= pVec.v[2];
		return *this;
	}

	Vec3 operator-(const float val) const {
		return Vec3(v[0] - val, v[1] - val, v[2] - val);
	}

	Vec3 operator-(const Vec3& v2) const {
		return Vec3(v[0] - v2.x, v[1] - v2.y, v[2] - v2.z);
	}

	Vec3& operator/=(const Vec3& pVec) {
		v[0] /= pVec.v[0];
		v[1] /= pVec.v[1];
		v[2] /= pVec.v[2];
		return *this;
	}

	Vec3 operator/(const float val) const {
		return Vec3(v[0] / val, v[1] / val, v[2] / val);
	}

	Vec3 operator/(const Vec3& v2) const {
		return Vec3(v[0] / v2.x, v[1] / v2.y, v[2] / v2.z);
	}

	Vec3 operator-() const {
		return Vec3(-v[0], -v[1], -v[2]);
	}

	float length() const {
		return sqrt((SQ(v[0]) + SQ(v[1]) + SQ(v[2])));
	}

	float lengthSquare() const {
		return SQ(v[0]) + SQ(v[1]) + SQ(v[2]);
	}

	Vec3 normalize() const {
		float len = 1.0f / length();
		return Vec3(x * len, y * len, z * len);
	}

	float normalize_GetLength() {
		float v_len = length();
		float len = 1.0f / v_len;
		v[0] *= len; v[1] *= len; v[2] *= len;
		return v_len;
	}

	float Dot(const Vec3& pVec) const {
		return v[0] * pVec.v[0] + v[1] * pVec.v[1] + v[2] * pVec.v[2];
	}

	Vec3 Cross(const Vec3& v1) const {
		return Vec3(v1.v[1] * v[2] - v1.v[2] * v[1],
			v1.v[2] * v[0] - v1.v[0] * v[2],
			v1.v[0] * v[1] - v1.v[1] * v[0]);
	}


	float Max() const {
		return std::max(v[0], std::max(v[1], v[2]));
	}

	float Min() const {
		return std::min(v[0], std::min(v[1], v[2]));
	}

	void printOut() {
		std::cout << v[0] << ", " << v[1] << ", " << v[2] << std::endl;
	}
};

class Vec4 {
public:
	Vec4(Vec3 v) : x(v.x), y(v.y), z(v.z), w(1) {}
	Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vec4() : x(0), y(0), z(0), w(1) {}

	union {
		float v[4];
		struct { float x, y, z, w; };
	};

	Vec4& operator+=(const Vec4& pVec) {
		v[0] += pVec.v[0];
		v[1] += pVec.v[1];
		v[2] += pVec.v[2];
		v[3] += pVec.v[3];
		return *this;
	}

	Vec4 operator+(const Vec4& pVec) const {
		return Vec4(v[0] + pVec.v[0], v[1] + pVec.v[1], v[2] + pVec.v[2], v[3] + pVec.v[3]);
	}

	Vec4& operator*=(const Vec4& pVec) {
		v[0] *= pVec.v[0];
		v[1] *= pVec.v[1];
		v[2] *= pVec.v[2];
		v[3] *= pVec.v[3];
		return *this;
	}

	Vec4 operator*(const float val) const {
		return Vec4(v[0] * val, v[1] * val, v[2] * val, v[3] * val);
	}

	Vec4& operator-=(const Vec4& pVec) {
		v[0] -= pVec.v[0];
		v[1] -= pVec.v[1];
		v[2] -= pVec.v[2];
		v[3] -= pVec.v[3];
		return *this;
	}

	Vec4 operator-(const float val) const {
		return Vec4(v[0] - val, v[1] - val, v[2] - val, v[3] - val);
	}

	Vec4& operator/=(const Vec4& pVec) {
		v[0] /= pVec.v[0];
		v[1] /= pVec.v[1];
		v[2] /= pVec.v[2];
		v[3] /= pVec.v[3];
		return *this;
	}

	Vec4 operator/(const float val) const {
		return Vec4(v[0] / val, v[1] / val, v[2] / val, v[3] / val);
	}

	Vec4 operator-() const {
		return Vec4(-v[0], -v[1], -v[2], -v[3]);
	}

	Vec4 divideByW() {
		return Vec4(x / w, y / w, z / w, 1.f/ w);
	}

	void printOut() {
		std::cout << v[0] << ", " << v[1] << ", " << v[2] << ", " << v[3] << std::endl;
	}
};

float Dot(const Vec3& v1, const Vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Max(const Vec3& v1, const Vec3& v2) {
	return Vec3(std::max(v1.v[0], v2.v[0]), std::max(v1.v[1], v2.v[1]), std::max(v1.v[2], v2.v[2]));
}

Vec3 Min(const Vec3& v1, const Vec3& v2) {
	return Vec3(std::min(v1.v[0], v2.v[0]), std::min(v1.v[1], v2.v[1]), std::min(v1.v[2], v2.v[2]));
}

class Matrix {
public:
	union {
		float a[4][4];
		float m[16];
	};

	void setIdentity() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				i == j ? a[i][j] = 1.f : a[i][j] = 0.f;
			}
		}
	}

	Matrix() {
		setIdentity();
	}

	Matrix(float _a, float _b, float _c, float _d, float _e, float _f, float _g, float _h, float _i, float _j, float _k, float _l, float _m, float _n, float _o, float _p) {
		m[0] = _a; m[1] = _b; m[2] = _c; m[3] = _d; m[4] = _e;
		m[5] = _f; m[6] = _g; m[7] = _h; m[8] = _i; m[9] = _j;
		m[10] = _k; m[11] = _l; m[12] = _m; m[13] = _n; m[14] = _o;
		m[15] = _p;
	}

	Vec4 mul(const Vec4& v) const {
		return Vec4(
			(v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11]),
			(v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]));
	}

	Vec3 mulPoint(const Vec3& v) const {
		Vec3 v1 = Vec3(
			(v.x * m[0] + v.y * m[1] + v.z * m[2]) + m[3],
			(v.x * m[4] + v.y * m[5] + v.z * m[6]) + m[7],
			(v.x * m[8] + v.y * m[9] + v.z * m[10]) + m[11]);
		return v1;
	}

	Vec4 mulPoint(const Vec4& v) const {
		Vec4 v1 = Vec4((v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w*m[3]),
			(v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7]),
			(v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w*m[11]), 
			(v.x * m[12] + v.y * m[13] + v.z * m[14]+ v.w*m[15]));
		return v1;
	}

	Vec3 mulVec(const Vec3& v) const {
		return Vec3((v.x * m[0] + v.y * m[1] + v.z * m[2]), (v.x * m[4] + v.y * m[5] + v.z * m[6]), (v.x * m[8] + v.y * m[9] + v.z * m[10]));
	}

	Matrix mul(const Matrix& matrix) const
	{
		Matrix ret;
		ret.m[0] = m[0] * matrix.m[0] + m[1] * matrix.m[4] + m[2] * matrix.m[8] + m[3] * matrix.m[12];
		ret.m[1] = m[0] * matrix.m[1] + m[1] * matrix.m[5] + m[2] * matrix.m[9] + m[3] * matrix.m[13];
		ret.m[2] = m[0] * matrix.m[2] + m[1] * matrix.m[6] + m[2] * matrix.m[10] + m[3] * matrix.m[14];
		ret.m[3] = m[0] * matrix.m[3] + m[1] * matrix.m[7] + m[2] * matrix.m[11] + m[3] * matrix.m[15];
		ret.m[4] = m[4] * matrix.m[0] + m[5] * matrix.m[4] + m[6] * matrix.m[8] + m[7] * matrix.m[12];
		ret.m[5] = m[4] * matrix.m[1] + m[5] * matrix.m[5] + m[6] * matrix.m[9] + m[7] * matrix.m[13];
		ret.m[6] = m[4] * matrix.m[2] + m[5] * matrix.m[6] + m[6] * matrix.m[10] + m[7] * matrix.m[14];
		ret.m[7] = m[4] * matrix.m[3] + m[5] * matrix.m[7] + m[6] * matrix.m[11] + m[7] * matrix.m[15];
		ret.m[8] = m[8] * matrix.m[0] + m[9] * matrix.m[4] + m[10] * matrix.m[8] + m[11] * matrix.m[12];
		ret.m[9] = m[8] * matrix.m[1] + m[9] * matrix.m[5] + m[10] * matrix.m[9] + m[11] * matrix.m[13];
		ret.m[10] = m[8] * matrix.m[2] + m[9] * matrix.m[6] + m[10] * matrix.m[10] + m[11] * matrix.m[14];
		ret.m[11] = m[8] * matrix.m[3] + m[9] * matrix.m[7] + m[10] * matrix.m[11] + m[11] * matrix.m[15];
		ret.m[12] = m[12] * matrix.m[0] + m[13] * matrix.m[4] + m[14] * matrix.m[8] + m[15] * matrix.m[12];
		ret.m[13] = m[12] * matrix.m[1] + m[13] * matrix.m[5] + m[14] * matrix.m[9] + m[15] * matrix.m[13];
		ret.m[14] = m[12] * matrix.m[2] + m[13] * matrix.m[6] + m[14] * matrix.m[10] + m[15] * matrix.m[14];
		ret.m[15] = m[12] * matrix.m[3] + m[13] * matrix.m[7] + m[14] * matrix.m[11] + m[15] * matrix.m[15];
		return ret;
	}

	Matrix rotateX(float angle) {
		Matrix m;
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		m[5] = cos; m[6] = -sin; m[9] = sin; m[10] = cos;
		return m;
	}

	Matrix rotateY(float angle) {
		Matrix m;
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		m.m[0] = cos; m.m[2] = sin; m.m[8] = -sin; m.m[10] = cos;
		return m;
	}

	Matrix rotateZ(float angle) const {
		Matrix m;
		float cos = std::cos(angle);
		float sin = std::sin(angle);
		m.m[0] = cos; m.m[1] = -sin; m.m[4] = sin; m.m[5] = cos;
		return m;
	}

	Matrix translate(Vec3& vec) const {
		Matrix m;
		m.a[0][3] = vec.x; m.a[1][3] = vec.y; m.a[2][3] = vec.z;
		return m;
	}

	Matrix scale(Vec3& vec) const {
		Matrix m;
		m.a[0][0] = vec.x; m.a[1][1] = vec.y; m.a[2][2] = vec.z;
		return m;
	}

	Matrix& transpose() {
		float temp;

		temp = a[0][1];
		a[0][1] = a[1][0];
		a[1][0] = temp;

		temp = a[0][2];
		a[0][2] = a[2][0];
		a[2][0] = temp;

		temp = a[0][3];
		a[0][3] = a[3][0];
		a[3][0] = temp;

		temp = a[2][1];
		a[2][1] = a[1][2];
		a[1][2] = temp;

		temp = a[3][1];
		a[3][1] = a[1][3];
		a[1][3] = temp;

		temp = a[2][3];
		a[2][3] = a[3][2];
		a[3][2] = temp;
		return *this;
	}

	Matrix projMatrix(float width, float height, float theta, float fov, float f, float n) {
		Matrix m;
		float aspect = width / height;
		float fovDeg = theta * fov;
		float fovRad = fovDeg * M_PI / 180.0f;
		float tanHalf = std::tan(fovRad * 0.5f);

		m.a[0][0] = 1.f/ (aspect * tanHalf);
		m.a[1][1] = 1.f / tanHalf;
		m.a[2][2] = f / (f - n);
		m.a[2][3] = -n * f / (f - n);
		m.a[3][2] = 1.f;
		m.a[3][3] = 0.f;
		return m;
	}

	Matrix lookAt(Vec3& to, Vec3& from, Vec3 up) {
		Vec3 dir = (to - from).normalize();
		Vec3 right = dir.Cross(up);
		Vec3 upPrime = right.Cross(dir);
		Matrix m;
		m.m[0] = right.x; m.m[1] = right.y; m.m[2] = right.z; m.m[3] = right.Dot(-from);
		m.m[4] = upPrime.x; m.m[5] = upPrime.y; m.m[6] = upPrime.z; m.m[7] = upPrime.Dot(-from);
		m.m[8] = dir.x; m.m[9] = dir.y; m.m[10] = dir.z; m.m[11] = dir.Dot(-from);

		return m;
	}

	float& operator[](int index) { return m[index]; }

	Matrix operator*(const Matrix& matrix) {
		return mul(matrix);
	}

	Matrix invert() {
		Matrix inv;
		inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];  inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];  inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];  inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];  inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];  inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];  inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];  inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];  inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];  inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];  inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];  inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];  inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];  inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];  inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];  inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];  float det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
		if (det == 0) {
		}
		det = 1.0 / det;
		for (int i = 0; i < 16; i++) {
			inv[i] = inv[i] * det;
		}
		return inv;
	}

	void printOut() {
		std::cout << a[0][0] << a[0][1] << a[0][2] << a[0][3] << "\n"
			<< a[1][0] << a[1][1] << a[1][2] << a[1][3] << "\n"
			<< a[2][0] << a[2][1] << a[2][2] << a[2][3] << "\n"
			<< a[3][0] << a[3][1] << a[3][2] << a[3][3] << "\n";
	}
};

class ShadingFrame {
public:
	Vec3 n;
	Vec3 t;
	Vec3 b;

	ShadingFrame(const Vec3& normal) {
		n = normal.normalize();

		Vec3 h = (std::fabs(n.x) > std::fabs(n.z))
			? Vec3(-n.y, n.x, 0.0f)
			: Vec3(0.0f, -n.z, n.y);

		float proj = h.Dot(n);
		Vec3 tCand = h - n * proj;
		t = tCand.normalize();

		b = n.Cross(t);
	}

	Vec3 toLocal(const Vec3& vWorld) const {
		return Vec3(
			vWorld.Dot(t),
			vWorld.Dot(b),
			vWorld.Dot(n)
		);
	}

	Vec3 toWorld(const Vec3& vLocal) const {
		return t * vLocal.x + b * vLocal.y + n * vLocal.z;
	}
};


class SphericalCoordinates {
public:
	float x, y, z, theta, phi, r;

	SphericalCoordinates(float _x, float _y, float _z) : x(_x), y(_y), z(_z), phi(0), theta(0), r(1) {}
	SphericalCoordinates(float _theta, float _phi) : theta(_theta), phi(_phi), r(1), x(0), y(0), z(0) {}
	SphericalCoordinates() : x(0), y(0), z(0), theta(0), phi(0), r(1) {}

	void cartesianVecZ() {
		theta = std::acos(z);
		phi = std::atan2f(y, x);
	}

	void cartesianVecY() {
		phi = std::atan2f(z, x);
	}

	void cartesianZ() {
		x = r * std::sin(theta) * std::cos(phi);
		y = r * std::sin(theta) * std::sin(phi);
		z = r * std::cos(theta);
	}

	void cartesianY() {
		x = r * std::sin(theta) * std::cos(phi);
		y = r * std::cos(theta);
		z = r * std::sin(theta) * std::sin(phi);
	}
};

class Quaternion {
public:
	Quaternion(float _a, float _b, float _c, float _d) : a(_a), b(_b), c(_c), d(_d) {}
	Quaternion() : a(0), b(0), c(0), d(1) {}

	union {
		struct { float a, b, c, d; };
		float q[4];
	};

	Quaternion conjugate() const {
		return Quaternion(-a, -b, -c, d);
	}

	float magnitude() const {
		return sqrt(SQ(a) + SQ(b) + SQ(c) + SQ(d));
	}

	Quaternion inverse() const {
		Quaternion conj = conjugate();
		float mag = magnitude();
		return Quaternion(conj.a / mag, conj.b / mag, conj.c / mag, conj.d / mag);
	}

	Quaternion normalise() const {
		float mag = magnitude();
		return Quaternion(a / mag, b / mag, c / mag, d / mag);
	}

	Quaternion operator-() const {
		return Quaternion(-a, -b, -c, -d);
	}

	Quaternion operator*(const Quaternion& q2) const {
		float i_sum = d * q2.a + a * q2.d + b * q2.c - c * q2.b;
		float j_sum = d * q2.b - a * q2.c + b * q2.d + c * q2.a;
		float k_sum = d * q2.c + a * q2.b - b * q2.a + c * q2.d;
		float scalar_sum = d * q2.d - a * q2.a - b * q2.b - c * q2.c;
		return Quaternion(i_sum, j_sum, k_sum, scalar_sum);
	}

	Quaternion fromAxisAngle(Vec3& v, float theta) const {
		float c = std::cos(theta / 2);
		float s = std::sin(theta / 2);
		return Quaternion(v.x * s, v.y * s, v.z * s, c);
	}

	float Dot(const Quaternion& q2) const {
		return (d * q2.d + a * q2.a + b * q2.b + c * q2.c);
	}

	Quaternion slerp(const Quaternion& q2, float t) const {
		float dot = Dot(q2);

		Quaternion q2_new = q2;
		if (dot < 0.0f) {
			dot = -dot;
			q2_new = -q2_new;
		}

		float theta = std::acos(dot);
		float sinTheta = std::sin(theta);
		float helper1 = theta * (1 - t);
		float helper2 = theta * t;
		float first = std::sin(helper1) / sinTheta;
		float second = std::sin(helper2) / sinTheta;
		return Quaternion(a * first + q2_new.a * second, b * first + q2_new.b * second, c * first + q2_new.c * second, d * first + q2_new.d * second);
	}

	Matrix toMatrix() {
		float aa = a * a, ab = a * b, ac = a * c;
		float bb = b * b, bc = b * c, cc = c * c;
		float da = d * a, db = d * b, dc = d * c;
		Matrix m;
		m[0] = 1 - 2 * (bb + cc); m[1] = 2 * (ab - dc); m[2] = 2 * (ac + db); m[3] = 0; m[4] = 2 * (ab + dc); m[5] = 1 - 2 * (aa + cc); m[6] = 2 * (bc - da); m[7] = 0; m[8] = 2 * (ac - db); m[9] = 2 * (bc + da); m[10] = 1 - 2 * (aa + bb); m[11] = 0; m[12] = m[13] = m[14] = 0; m[15] = 1;
		return m;
	}

};

class Colour {
public:
	Colour(float _r, float _g, float _b) : r(_r), g(_g), b(_b){}
	Colour() : r(0), g(0), b(0) {}
	Colour(unsigned char input_r, unsigned char input_g, unsigned char input_b) {
		r = input_r / 255.f;
		g = input_g / 255.f;
		b = input_b / 255.f;
	}

	union {
		float c[3];
		struct {
			float r, g, b;
		};
	};

	Colour operator+(const Colour& colour) const {
		return Colour(r + colour.r, g + colour.g, b + colour.b);
	}

	Colour operator*(const Colour& colour) const {
		return Colour(colour.r * r, colour.g * g, colour.b * b);
	}

	Colour operator*(const float val) const {
		return Colour(r * val, g * val, b * val);
	}

	Colour operator/(const float val) const {
		return Colour(r / val, g / val, b / val);
	}
};