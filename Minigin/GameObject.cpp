#pragma once
#include "GameObject.h"
//#include "SpriteRenderer.h"
//#include "Collider.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "TextureRenderer.h"
#include "Console.h"

std::vector<std::unique_ptr<GameObject>> GameObject::objToCreate{};
std::vector<GameObject*> GameObject::objToDelete{};

GameObject* GameObject::Create(std::string name)
{
	GameObject* obj = new GameObject(name);
	objToCreate.emplace_back(std::unique_ptr<GameObject>(obj));
	return obj;
}

void GameObject::CreateObjects(Scene* curScene)
{
	for (auto& obj : objToCreate) {
		obj->SetInternActive(obj->GetActive());
		curScene->Add(std::move(obj));
	}
	objToCreate.clear();
}

bool GameObject::Delete(GameObject* obj)
{

	if (std::find(objToDelete.begin(), objToDelete.end(), obj) == objToDelete.end()) {

		EventManager::GetInstance().RemoveListener(obj);
		if (SceneManager::GetInstance().curScene->FindObj([&](GameObject* gameobj) {return gameobj == obj; }) != nullptr) {
			objToDelete.emplace_back(obj);
			obj->deleted = true;
			for (auto& child : obj->childrenPtr) {
				Delete(child);
				//objToDelete.emplace_back(child);
			}
			if (obj->parentPtr != nullptr) {
				//obj->SetParent(nullptr);
			}
		}
		else if (std::find_if(objToCreate.begin(), objToCreate.end(), [obj](std::unique_ptr<GameObject>& o) {return o.get() == obj; }) != objToCreate.end()) {
			obj->deleted = true;
			obj->SetParent(nullptr);
			for (auto& child : obj->childrenPtr) {
				Delete(child);
				//objToDelete.emplace_back(child);
			}
			//objToDelete.emplace_back(obj);
			objToCreate.erase(std::find_if(objToCreate.begin(), objToCreate.end(), [obj](std::unique_ptr<GameObject>& o) {return o.get() == obj; }));
		}
		return true;
	}
	return false;
}

void GameObject::DeleteObjects(Scene* curScene)
{
	for (auto& obj : objToDelete) {
		obj->SetParent(nullptr);
	}
	for (auto& obj : objToDelete) {
		curScene->Remove(obj);
	}
	objToDelete.clear();

	//auto iter = std::find_if(objToCreate.begin(), objToCreate.end(), [&](std::unique_ptr<GameObject>& o) {return o.get()->IsDeleted(); });
	//while (iter != objToCreate.end()) {
	//	objToCreate.erase(iter);
	//	iter = std::find_if(objToCreate.begin(), objToCreate.end(), [&](std::unique_ptr<GameObject>& o) {return o.get()->IsDeleted(); });
	//}
}

GameObject::GameObject(std::string name, const Vector3& pos, const Vector3& scale, float rotation, GameObject* parent):
	name{name},
	parentPtr{parent},
	childrenPtr{},
	prevEnabled{enabled}
{
	Init();
	transform->position = pos;
	transform->scale = scale;
	transform->rotation = rotation;
}

GameObject::GameObject(std::string name):
	name{name },
	parentPtr{  },
	childrenPtr{},
	prevEnabled{ enabled }
{
	Init();
}

GameObject::GameObject():
	name{ },
	parentPtr{  },
	childrenPtr{},
	prevEnabled{ enabled }
{
	Init();
}

GameObject::~GameObject() {}

void GameObject::Init()
{
	//SceneManager::GetInstance().curScene->Add(std::shared_ptr<GameObject>(this));
	if (!GetComponent<Transform>()) {
		transform = AddComponent<Transform>();
	}
}


void GameObject::Update()
{
	if (deleted) return;
	if (!prevEnabled && enabled) {
		for (size_t idx{}; idx < components.size(); idx++) {
			components[idx]->OnEnable();
		}
		prevEnabled = enabled;
	}
	if (prevEnabled && !enabled) {
		for (size_t idx{}; idx < components.size(); idx++) {
			components[idx]->OnDisable();
		}
		prevEnabled = enabled;
	}
	if (enabled) {
		for (size_t idx{}; idx < newComponents.size(); idx++) {
			newComponents[idx]->Start();
			//newComponents.DeleteAt(idx);
			//return;
		}
		newComponents.clear();

		for (size_t idx{}; idx < components.size(); idx++) {
			components[idx]->CheckGO();
			components[idx]->Update();
		}
		if (transform == nullptr) return;
		//if (transform->position != transform->prevPos) {
		//	for (int idx{}; idx < childrenPtr.Size(); idx++) {
		//		childrenPtr[idx]->transform->position += transform->position - transform->prevPos;
		//	}
		//	transform->prevPos = transform->position;
		//}
	}

}

void GameObject::Render()
{
	if (deleted) return;
	if (enabled) {
		for (size_t idx{}; idx < components.size(); idx++) {
			components[idx]->Render();
		}
	}

}

void GameObject::Render(int renderOrder)
{
	renderOrder++;
	if (!enabled) return;
	//SpriteRenderer* spRender = GetComponent <SpriteRenderer>();
	//if (spRender != nullptr && spRender->GetOrder() == order) {
	//	spRender->Render();
	//}
	//Collider* col = GetComponent <Collider>();
	//if (col != NULL) col->Render();
	// 
	//List<SpriteRenderer*> spRender = GetAllComponents<SpriteRenderer>();
	//std::vector<SpriteRenderer*> drawSprites = spRender.FindAll([order](SpriteRenderer* sp) { return sp->order == order; });
	//for (int idx{}; idx < drawSprites.size(); idx++) {
	//	drawSprites[idx]->Draw();
	//}
}

void GameObject::RenderUI()
{
	if (enabled) {
		for (size_t idx{}; idx < components.size(); idx++) {
			components[idx]->RenderUI();
		}
	}
}

void GameObject::SetParent(GameObject* parent)
{
	if (parent == nullptr && parentPtr != nullptr) {
		parentPtr->childrenPtr.erase(std::remove_if(parentPtr->childrenPtr.begin(), parentPtr->childrenPtr.end(),([&](GameObject* go) {return go == this; })),
			parentPtr->childrenPtr.end());
		parentPtr = nullptr;
	}
	if (parent == nullptr) {
		transform->localPosition = transform->position;
		return;
	}
	transform->localPosition = transform->position - parent->transform->position;

	parentPtr = parent;
	parentPtr->childrenPtr.emplace_back(this);
	transform->SetScale(parentPtr->transform->GetScale());

	transform->isDirty = true;
}

void GameObject::SetActive(bool active) {
	enabled = active;
	localEnabled = active;
	for (auto& child : childrenPtr) {
		child->SetInternActive(active);
	}
}

void GameObject::SetInternActive(bool active)
{
	if (!active || !localEnabled) {
		enabled = false;
		for (auto& child : childrenPtr) {
			child->SetInternActive(false);
		}
		return;
	}
	if ((localEnabled && active) || (!localEnabled && !active)) {
		SetActive(active);
	}


}

bool GameObject::GetActive()
{
	if (parentPtr != nullptr) {
		if (!enabled) {
			return false;
		}
	}
	if (parentPtr == nullptr) return enabled;
	return parentPtr->GetActive();
}

//GameObject* GameObject::FindChild(std::string cname)
//{
//	return childrenPtr.Find([cname](std::shared_ptr<GameObject> g) {return g->name == cname; }).get();
//}

//void GameObject::Copy(GameObject other, bool parent)
//{
//	if (parent) name = other.name + " (copy)";
//	else name = other.name;
//	enabled = other.enabled;
//	transform->position = other.transform->position;
//	transform->localPosition = other.transform->localPosition;
//	transform->scale = other.transform->scale;
//	transform->localScale = other.transform->localScale;
//	transform->rotation = other.transform->rotation;
//	transform->localRotation = other.transform->localRotation;
//
//	for (int idx{}; idx < other.components.Size(); idx++) {
//
//		AddComponentCopy(other.components[idx].get()); // ???
//		//components.Add(new decltype(other.components[idx])());
//	}
//
//	parentPtr = other.parentPtr;
//	for (int idx{}; idx < other.childrenPtr.Size(); idx++) {
//		std::shared_ptr<GameObject> child = std::make_shared<GameObject>(new GameObject());
//		child->Copy(*other.childrenPtr[idx], false);
//		childrenPtr.Add(child);
//	}
//}
