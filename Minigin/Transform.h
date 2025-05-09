#pragma once
#include <glm.hpp>
#include "BaseComponent.h"
#include "VarWrapper.h"

class Transform : public BaseComponent
{
	friend VarWrapper<glm::vec3, Transform>;
public:
	//typedef glm::vec3(*get)(glm::vec3);

	Vector3 GetTrueLocalTest();
	Vector3& GetLocalPosition();
	Vector3& GetPosition();
	Vector3& GetScale();

	VarWrapper<Vector3, Transform> position{ this, [this]() { return (this->GetPosition()); }, [this](const Vector3& v) { this->SetPosition(v); } };
	VarWrapper<Vector3, Transform> localPosition{this,[this]() { return (this->GetLocalPosition()); },[this](const Vector3& v) { this->SetLocalPosition(v); } };
	VarWrapper<Vector3, Transform> scale{ this, [this]() { return (this->GetScale()); }, [this](const Vector3& v) { this->SetScale(v); }, Vector3(1,1,1)};
	VarWrapper<Vector3, Transform> localScale{ this, [this]() { return (this->GetScale()); }, [this](const Vector3& v) { this->SetLocalScale(v); }, Vector3(1,1,1) };

	float rotation{};
	float localRotation{}; // no quaternion wackiness, rotation won't be used much

	Vector3 prevPos{}; //?

	bool isDirty{ false };

	void Start() override;
	void Update() override;
	void SetPosition(const Vector3&);
	void SetLocalPosition(const Vector3&);
	void SetScale(const Vector3&);
	void SetLocalScale(const Vector3&);
	void SetDirty();
	bool GetDirty();
protected:
	// using these functions kinda screws everything up, so they are protected
	// the varwrapper takes care of everything... I hope
	void SetPosition(float x, float y, float z);
	void SetLocalPosition(float x, float y, float z);
private:
};

