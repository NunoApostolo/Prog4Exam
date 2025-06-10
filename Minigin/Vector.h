#pragma once
#include <string>

class Vector3 {
public:
	Vector3() : x{}, y{}, z{} {}
	Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
	
	float x;
	float y;
	float z;

	const Vector3 operator+(const Vector3& rhs);
	const Vector3 operator+(const Vector3& rhs) const;
	const Vector3 operator-(const Vector3& rhs);
	const Vector3 operator-(const Vector3& rhs) const;
	const Vector3 operator*(const float rhs) const;
	const Vector3 operator*(const Vector3& rhs);
	const Vector3 operator*(const Vector3& rhs) const;
	const Vector3 operator/(const float rhs);
	const Vector3 operator/(const float rhs) const;
	Vector3 operator/(const Vector3& rhs);
	const Vector3 operator/(const Vector3& rhs) const;
	Vector3& operator+=(const Vector3& rhs);
	bool operator==(const Vector3& rhs) const;
	bool operator!=(const Vector3& rhs);
	Vector3 Normalize() const;
	static float Length(const Vector3& v);
	static float Distance(const Vector3& v1, const Vector3& v2);
	static float Angle(const Vector3& v1, const Vector3& v2);
	static float AngleDeg(const Vector3& v1, const Vector3& v2);
};

class Vector2 : public Vector3 {
public:
	Vector2() : Vector3() {}
	Vector2(float x, float y) : Vector3(x, y, 0) {}

	const Vector2 operator+(const Vector2& rhs) {
		return Vector2(x + rhs.x, y + rhs.y);
	}
	const Vector2 operator+(const Vector3& rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}
	const Vector2 operator-(const Vector2& rhs) {
		return Vector2(x - rhs.x, y - rhs.y);
	}
	const Vector2 operator-(const Vector3& rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}
	const Vector2 operator*(const float rhs) {
		return Vector2(x * rhs, y * rhs);
	}
	const Vector2 operator*(const Vector3& rhs) {
		return Vector2(x * rhs.x, y * rhs.y);
	}
	const Vector2 operator/(const float rhs) {
		return Vector2(x / rhs, y / rhs);
	}
};

class Rect {
public:
	Rect() : width{}, height{} {}
	Rect(Vector2 size) : width{ static_cast<int>(size.x) }, height{ static_cast<int>(size.y) } {}
	Rect(int width, int height) : width{ width }, height{ height } {}
	int width;
	int height;
};

inline const std::string operator+(std::string str, const Vector3& rhs) {
	std::string strres{ str + "Vector3(" + std::to_string(rhs.x) + ", " + std::to_string(rhs.y) + ", " + std::to_string(rhs.z) + ")"};
	return strres;
}
inline const std::string operator+(std::string str, int rhs) {
	std::string strres{ str + std::to_string(rhs)};
	return strres;
}