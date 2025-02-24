#include "GameObject.h"
//#include "SpriteRenderer.h"
//#include "Collider.h"
#include "SceneManager.h"
#include "Scene.h"
#include "BaseComponent.h"
#include "Transform.h"
#include "TextureRenderer.h"

std::shared_ptr<GameObject> GameObject::CreateObject(std::string name)
{
	auto scene = SceneManager::GetInstance().curScene;
	auto go = std::make_shared<GameObject>(name);
	scene->Add(go);
	return go;
}

GameObject::GameObject(std::string name, const glm::vec3& pos, const glm::vec3& scale, float rotation, GameObject* parent):
	name{name},
	enabled{true},
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
	enabled{ true },
	parentPtr{  },
	childrenPtr{},
	prevEnabled{ enabled }
{
	Init();
}

GameObject::GameObject():
	name{ },
	enabled{ true },
	parentPtr{  },
	childrenPtr{},
	prevEnabled{ enabled }
{
	Init();
}

GameObject::~GameObject()
{
	for (int idx{}; idx < components.Size(); idx++) {
		components[idx].reset();
	}
	//components.CleanUp();
	//newComponents.CleanUp();
}

void GameObject::Init()
{
	//SceneManager::GetInstance().curScene->Add(std::shared_ptr<GameObject>(this));
	if (!GetComponent<Transform>()) {
		transform = AddComponent<Transform>();
	}
}


void GameObject::Update()
{
	if (!prevEnabled && enabled) {
		for (int idx{}; idx < components.Size(); idx++) {
			components[idx]->OnEnable();
		}
		prevEnabled = enabled;
	}
	if (prevEnabled && !enabled) {
		for (int idx{}; idx < components.Size(); idx++) {
			components[idx]->OnDisable();
		}
		prevEnabled = enabled;
	}
	if (enabled) {
		for (int idx{}; idx < newComponents.Size(); idx++) {
			newComponents[idx]->Start();
			newComponents.DeleteAt(idx);
			//return;
		}

		for (int idx{}; idx < components.Size(); idx++) {
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
	if (enabled) {
		for (int idx{}; idx < components.Size(); idx++) {
			components[idx]->Render();
		}
	}

}

void GameObject::Render(int order)
{
	order++;
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

void GameObject::SetParent(GameObject* parent)
{
	if (parentPtr != nullptr) {
		parentPtr->childrenPtr.Delete([&](std::shared_ptr<GameObject> go) {return go.get() == this; });
		if (parent == nullptr) parentPtr = nullptr;
	}
	if (parent == nullptr) return;
	glm::vec3 prevPos{transform->position};
	parentPtr = parent;
	parentPtr->childrenPtr.Add(SceneManager::GetInstance().curScene->GetObjPtr(this));
	transform->localPosition = prevPos - parentPtr->transform->position;

	transform->isDirty = true;
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
