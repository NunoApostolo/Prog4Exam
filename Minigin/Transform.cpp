#include "Transform.h"
#include "GameObject.h"

glm::vec3& Transform::GetPosition() // do not assign possition with = operator here -> endless recursion lol
{
	if (isDirty) {
		if (gameObject->parentPtr != nullptr) {
			glm::vec3 pPos{ gameObject->parentPtr->transform->GetPosition() };
			SetPosition(localPosition + pPos);
		}
		else SetPosition(localPosition);
		isDirty = false;
	}

	return position;
}

void Transform::SetPosition(const float x, const float y, const float z)
{
	SetPosition(glm::vec3(x, y, z));
}
void Transform::SetPosition(const glm::vec3& pos) // same as local if no parent
{
	isDirty = true;
	position.x = pos.x;
	position.y = pos.y;
	position.z = pos.z;
	if (gameObject->parentPtr == nullptr) {
		localPosition.x = position.x;
		localPosition.y = position.y;
		localPosition.z = position.z;
	}
	else {
		localPosition.x = position.x - gameObject->parentPtr->transform->position.x;
		localPosition.y = position.y - gameObject->parentPtr->transform->position.y;
		localPosition.z = position.z - gameObject->parentPtr->transform->position.z;
		localPosition.cache = localPosition;
	}

}

void Transform::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition(glm::vec3(x, y, z));
}

void Transform::SetLocalPosition(const glm::vec3& pos)
{
	isDirty = true;
	localPosition.x = pos.x;
	localPosition.y = pos.y;
	localPosition.z = pos.z;
}

void Transform::Start()
{
	//position.getter = [this]() { return (this->GetPosition()); };
	//position.setter = [this](glm::vec3 v) { this->SetPosition(v); };
}

void Transform::Update()
{
	position.CheckCache(static_cast<glm::vec3>(position));
	localPosition.CheckCache(static_cast<glm::vec3>(localPosition));
}
