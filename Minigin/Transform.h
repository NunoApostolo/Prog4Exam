#pragma once
#include <glm.hpp>
#include "BaseComponent.h"

class Transform : public BaseComponent
{
public:
	Transform(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	const glm::vec3& GetPosition() const { return position; }
	void SetPosition(float x, float y, float z);
	glm::vec3 position{}; // global position
	glm::vec3 localPosition{}; // global + parent position
	glm::vec3 scale{};
	glm::vec3 localScale{};

	float rotation{};
	float localRotation{}; // no quaternion wackiness, rotation won't be used much

	glm::vec3 prevPos{}; //?
private:

};

