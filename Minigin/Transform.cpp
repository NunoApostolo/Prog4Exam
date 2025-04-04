#include "Transform.h"
#include "GameObject.h"

Vector3& Transform::GetPosition() // do not assign possition with = operator here -> endless recursion lol
{
	if (isDirty) {
		if (gameObject->parentPtr != nullptr) {
			Vector3 pPos{ gameObject->parentPtr->transform->GetPosition() };
			SetPosition(localPosition + pPos);
		}
		else SetPosition(localPosition);
		isDirty = false;
	}

	return position;
}

void Transform::SetPosition(const float x, const float y, const float z)
{
	SetPosition(Vector3(x, y, z));
}
void Transform::SetPosition(const Vector3& pos) // same as local if no parent
{
	isDirty = true;
	position.SetVec3(pos);
	position.cache = pos;
	if (gameObject->parentPtr == nullptr) {
		localPosition.SetVec3(position);
	}
	else {
		localPosition.SetVec3(static_cast<Vector3>(position) - static_cast<Vector3>(gameObject->parentPtr->transform->position));
		localPosition.cache = localPosition;
	}

}

void Transform::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition(Vector3(x, y, z));
}

void Transform::SetLocalPosition(const Vector3& pos)
{
	isDirty = true;
	localPosition.SetVec3(pos);
	localPosition.cache = localPosition;
}

void Transform::Start()
{
	//position.getter = [this]() { return (this->GetPosition()); };
	//position.setter = [this](glm::vec3 v) { this->SetPosition(v); };
}

void Transform::Update()
{
	position.CheckCache(static_cast<Vector3>(position));
	localPosition.CheckCache(static_cast<Vector3>(localPosition));
}
