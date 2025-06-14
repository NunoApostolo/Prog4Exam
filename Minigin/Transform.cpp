#pragma once
#include "Transform.h"
#include "GameObject.h"
#include "Console.h"

Vector3 Transform::GetTrueLocalTest() {
	if (gameObject->parentPtr != nullptr) {
		return (static_cast<Vector3>(gameObject->parentPtr->transform->position) - static_cast<Vector3>(position)) 
			/ gameObject->parentPtr->transform->GetScale();
	}
	else return position;
}

Vector3& Transform::GetPosition() // do not assign possition with = operator here -> endless recursion lol
{
	if (isDirty) {
		if (gameObject->parentPtr != nullptr) {
			//Vector3 pPos{ gameObject->parentPtr->transform->GetPosition() };
			////Vector3 pos{ GetTrueLocalTest()};
			//Vector3 pos{ localPosition };
			//SetPosition((pos + pPos));

			Vector3 pPos{ gameObject->parentPtr->transform->GetPosition() };
			Vector3 pos{ localPosition };
			SetPosition(pos * gameObject->parentPtr->transform->GetScale() + pPos);
			GetScale();

		}
		else SetPosition(localPosition);
		isDirty = false;
		//Console::GetInstance().Log(gameObject->name + ":" + position);
	}

	return position;
}
Vector3& Transform::GetLocalPosition()
{
	GetPosition();
	return localPosition;

}
Vector3& Transform::GetScale() {
	//if (isDirty) {
	//	if (gameObject->parentPtr != nullptr) {
	//		Vector3 pScale{ gameObject->parentPtr->transform->GetScale() };
	//		Vector3 sscale{ localScale };
	//		SetScale(sscale * pScale);
	//	}
	//	else SetScale(localScale);
	//	isDirty = false;
	//	Console::GetInstance().Log(gameObject->name + ":Scale:" + scale);
	//}
	if (gameObject->parentPtr != nullptr) {
		scale.SetVec3(Vector3(1,1,1) * localScale * gameObject->parentPtr->transform->GetScale());
		return scale;
	}
	else {

	}
	return localScale;
}


void Transform::SetPosition(const float x, const float y, const float z)
{
	SetPosition(Vector3(x, y, z));
}
void Transform::SetPosition(const Vector3& pos) // same as local if no parent
{
	isDirty = true;
	if (gameObject->parentPtr == nullptr) {
		position.SetVec3(pos);
		localPosition.SetVec3(position);
	}
	else {
		localPosition.SetVec3((static_cast<Vector3>(pos) -
			static_cast<Vector3>(gameObject->parentPtr->transform->GetPosition()))
			/ gameObject->parentPtr->transform->GetScale());

		position.SetVec3(pos);

	}
	for (auto& child : gameObject->childrenPtr) {
		child->transform->SetDirty();
	}
}

void Transform::SetLocalPosition(float x, float y, float z)
{
	SetLocalPosition(Vector3(x, y, z));
}

void Transform::SetLocalPosition(const Vector3& pos)
{
	isDirty = true;
	//Vector3 scaledPos = pos;
	localPosition.SetVec3(pos);
	if (gameObject->parentPtr != nullptr) {
		//localPosition.SetVec3(scaledPos * gameObject->parentPtr->transform->GetScale());
	}
	//else localPosition.SetVec3(scaledPos);

	if (gameObject->parentPtr != nullptr) {
		//position.SetVec3(static_cast<Vector3>(localPosition) * gameObject->parentPtr->transform->GetScale() 
			//+ static_cast<Vector3>(gameObject->parentPtr->transform->position));
	}
	for (auto& child : gameObject->childrenPtr) {
		child->transform->SetDirty();
	}
}

void Transform::SetScale(const Vector3& scl)
{
	this->scale.SetVec3(scl);
	if (gameObject->parentPtr == nullptr) {
		localScale.SetVec3(scl);
	}

	for (auto& child : gameObject->childrenPtr) {
		child->transform->SetScale(scale);
		child->transform->SetDirty();
		//child->transform->SetLocalPosition(child->transform->localPosition);
	}
	//Console::GetInstance().Log(gameObject->name + "Scale:" + scale);
}

void Transform::SetLocalScale(const Vector3& scl)
{
	isDirty = true;
	localScale.SetVec3(scl);

	if (gameObject->parentPtr != nullptr) {
		//this->scale.SetVec3(Vector3(1, 1, 1) * static_cast<Vector3>(gameObject->parentPtr->transform->GetScale()) * static_cast<Vector3>(localScale));
	}

	for (auto& child : gameObject->childrenPtr) {
		child->transform->SetDirty();
		//child->transform->SetScale(Vector3(1,1,1)*GetScale()*child->transform->localScale);
		//child->transform->SetLocalPosition(child->transform->localPosition);
	}
}

void Transform::SetDirty()
{
	for (auto& c : gameObject->childrenPtr) {
		c->transform->SetDirty();
	}
	isDirty = true;
}

bool Transform::GetDirty()
{
	if (gameObject->parentPtr != nullptr) {
		return gameObject->parentPtr->transform->GetDirty();
	}
	return isDirty;
}

void Transform::Start()
{
	//position.getter = [this]() { return (this->GetPosition()); };
	//position.setter = [this](glm::vec3 v) { this->SetPosition(v); };
}

void Transform::Update()
{
	if (isDirty) {
		GetPosition();
		GetScale();
		for (size_t idx{}; idx < gameObject->childrenPtr.size(); idx++) {
			gameObject->childrenPtr[idx]->transform->isDirty = true;
		}
	}
	//position.CheckCache(static_cast<Vector3>(position));
	//localPosition.CheckCache(static_cast<Vector3>(localPosition));
}
