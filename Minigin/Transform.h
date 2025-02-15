#pragma once
#include <glm.hpp>
#include "BaseComponent.h"

using namespace glm;
class Transform : public BaseComponent
{
public:
	Transform(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	const vec3& GetPosition() const { return position; }
	void SetPosition(float x, float y, float z);
	vec3 position{}; // global position
	vec3 localPosition{}; // global + parent position
	vec3 scale{};
	vec3 localScale{};

	float rotation{};
	float localRotation{}; // no quaternion wackiness, rotation won't be used much

	vec3 prevPos{}; //?
private:

};

