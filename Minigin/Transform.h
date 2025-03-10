#pragma once
#include <glm.hpp>
#include "BaseComponent.h"
#include "VarWrapper.h"

class Transform : public BaseComponent
{
public:
	Transform(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	//typedef glm::vec3(*get)(glm::vec3);

	glm::vec3& GetPosition();

	VarWrapper<glm::vec3, Transform> position{ this, [this]() { return (this->GetPosition()); }, [this](glm::vec3 v) { this->SetPosition(v); } };
	VarWrapper<glm::vec3, Transform> localPosition{this,[this]() { return (this->GetPosition()); },[this](const glm::vec3& v) { this->SetLocalPosition(v); } };
	glm::vec3 scale{};
	glm::vec3 localScale{};

	float rotation{};
	float localRotation{}; // no quaternion wackiness, rotation won't be used much

	glm::vec3 prevPos{}; //?

	bool isDirty{ false };

	void Start() override;
	void Update() override;
	void SetPosition(const glm::vec3&);
	void SetLocalPosition(const glm::vec3&);
protected:
	// using these functions kinda screws everything up, so they are protected
	// the varwrapper takes care of everything... I hope
	void SetPosition(float x, float y, float z);
	void SetLocalPosition(float x, float y, float z);
private:
};

