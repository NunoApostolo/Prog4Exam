#pragma once

class Vector3 {
public:
	Vector3() : x{}, y{}, z{} {}
	Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}
	
	float x;
	float y;
	float z;

	const Vector3 operator+(const Vector3& rhs) {
		return Vector3(x+rhs.x,y+rhs.y,z+rhs.z);
	}
	const Vector3 operator-(const Vector3& rhs) {
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	const Vector3 operator*(const float rhs) {
		return Vector3(x * rhs, y * rhs, z * rhs);
	}
	const Vector3 operator/(const float rhs) {
		return Vector3(x / rhs, y / rhs, z / rhs);
	}
	bool operator!=(const Vector3& rhs) {
		
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}


};

class Vector2 : public Vector3 {
public:
	Vector2() : Vector3() {}
	Vector2(float x, float y) : Vector3(x, y, 0) {}
};