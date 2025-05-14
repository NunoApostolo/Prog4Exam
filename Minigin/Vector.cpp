#include "Vector.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

const Vector3 Vector3::operator+(const Vector3& rhs) {
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}
const Vector3 Vector3::operator+(const Vector3& rhs) const {
	return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
}
const Vector3 Vector3::operator-(const Vector3& rhs) {
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}
const Vector3 Vector3::operator-(const Vector3& rhs) const {
	return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
}
const Vector3 Vector3::operator*(const float rhs) const {
	return Vector3(x * rhs, y * rhs, z * rhs);
}
const Vector3 Vector3::operator*(const Vector3& rhs) {
	return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
}
const Vector3 Vector3::operator*(const Vector3& rhs) const {
	return Vector3(x * rhs.x, y * rhs.y, z * rhs.z);
}
const Vector3 Vector3::operator/(const float rhs) {
	return Vector3(x / rhs, y / rhs, z / rhs);
}
const Vector3 Vector3::operator/(const float rhs) const {
	return Vector3(x / rhs, y / rhs, z / rhs);
}
Vector3 Vector3::operator/(const Vector3& rhs) {
	return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
}
const Vector3 Vector3::operator/(const Vector3& rhs) const {
	return Vector3(x / rhs.x, y / rhs.y, z / rhs.z);
}
Vector3& Vector3::operator+=(const Vector3& rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	return *this;
}
bool Vector3::operator==(const Vector3& rhs) {

	return x == rhs.x && y == rhs.y && z == rhs.z;
}
bool Vector3::operator!=(const Vector3& rhs) {

	return !(x == rhs.x && y == rhs.y && z == rhs.z);
}

Vector3 Vector3::Normalize() const {
	float c{ std::sqrtf(std::powf(x,2) + std::powf(y,2) + std::powf(z,2)) };
	return Vector3(x / c, y / c, z / c);
}
float Vector3::Length(const Vector3& v)
{
	return Distance(Vector3(), v);
}
float Vector3::Distance(const Vector3& v1, const Vector3& v2) {
	Vector3 diff{ v2 - v1 };
	return std::sqrtf(std::powf(diff.x, 2) + std::powf(diff.y, 2) + std::powf(diff.z, 2));
}
float Vector3::Angle(const Vector3& v1, const Vector3& v2) {
	Vector3 diff{ v2 - v1 };
	return std::atanf(diff.y / diff.x);
}
float Vector3::AngleDeg(const Vector3& v1, const Vector3& v2) {
	//this is ass edit: no longer ass :)
	Vector3 base{ 100,0,0 };
	Vector3 diff{ v2 - v1 };
	float dot{ base.x*diff.x + base.y*diff.y + base.z*diff.z};
	float res = std::acosf(dot / (Length(base) * Length(diff))) / static_cast<float>(M_PI) * 180.f;
	if (diff.y < 0) res += 180;
	return res;
}