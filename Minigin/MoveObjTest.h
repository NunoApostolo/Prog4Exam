#include "BaseComponent.h"
#include "Transform.h"

#pragma once
class MoveObjTest : public BaseComponent
{
public:
	MoveObjTest(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	void Start() override;
	void Update() override;
	void Render() const override;
private:
	float angle{0};
	glm::vec3 startPos{};
};

