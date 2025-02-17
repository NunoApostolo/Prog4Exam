#include <string>
#include <memory>
#include "List.h"
#include "BaseComponent.h"
//#include <utils.h>
#include "Time.h"
//#include "Camera.h"
//#include "ColliderManager.h"
#include <glm.hpp>
//#include "Transform.h"

class Transform;
#pragma once
class GameObject final
{
public:
	std::string name;
	bool enabled;
	Transform* transform;

	GameObject* parentPtr;
	List<std::shared_ptr<GameObject>> childrenPtr; // framework won't be used much

	GameObject(std::string name, const glm::vec3& pos, const glm::vec3& scale, float rotarion, GameObject* parent = nullptr); // just in case, different constructor behaviour
	GameObject(std::string name);
	GameObject();
	GameObject& operator =(const GameObject&) = delete;
	GameObject operator =(GameObject) = delete;
	~GameObject();
	void Init();

	void Update(); // updates components
	void Render();// draws components
	void Render(int order);// draws components

	//GameObject* FindChild(std::string cname);

	template <typename T> // dynamic? / not seperating template code from header. It's a pain otherwise.
	T* AddComponent() {
		GameObject* ptr{ this };
		std::shared_ptr<T> comp = std::make_shared<T>(ptr, typeid(T).name());
		//T* comp{ new T(ptr, typeid(T).name()) };
		//std::shared_ptr<T> comp(new T(ptr, typeid(T).name()));

		components.Add(comp);
		newComponents.Add(comp);
		comp->Awake();
		return comp.get();
	}
	template <typename T>
	T* GetComponent() {
		for (int idx{}; idx < components.Size(); idx++) {
			T* comp{ dynamic_cast<T*>(components[idx].get()) };
			if (comp != nullptr) {
				return comp;
			}
		}
		//for (int idx{}; idx < components.Size(); idx++) {
		//	if (components[idx]->GetType() == typeid(T).name()) {
		//		T* comp{ (T*)components[idx] };
		//		return comp;
		//	}
		//}
		return nullptr;
	}
	template <typename T>
	std::list<T*> GetAllComponents() {
		List<T*> list{};
		for (int idx{}; idx < components.Size(); idx++) {
			if (components[idx]->GetType() == typeid(T).name()) {
				list.Add((T*)components[idx]);
			}
		}
		return list;
	}

	List<std::shared_ptr<BaseComponent>> GetAllComponents() {
		return components;
	}
	template <typename T>
	T* GetComponentInChildren() {
		for (int cidx{}; cidx < childrenPtr.Size(); cidx++) {
			for (int idx{}; idx < childrenPtr[cidx]->components.Size(); idx++) {
				if (childrenPtr[cidx]->components[idx]->GetType() == typeid(T).name()) {
					T* comp{ (T*)childrenPtr[cidx]->components[idx] };
					return comp;
				}
			}
		}
		return nullptr;
	}
	template <typename T>
	T* GetComponentInParent() {
		for (int idx{}; idx < components.Size(); idx++) {
			if (components[idx]->GetType() == typeid(T).name()) {
				T* comp{ (T*)components[idx] };
				return comp;
			}
		}
		if (parentPtr != nullptr) {
			return parentPtr->GetComponentInParent<T>();
		}
		return nullptr;
	}
	template <typename T>
	void DeleteComponent() {
		for (int idx{}; idx < components.Size(); idx++) {
			if (components[idx]->GetType() == typeid(T).name()) {
				//Collider* col{ dynamic_cast<Collider*>(components[idx]) };
				//if (col != nullptr) {
				//	ColliderManager::RemoveObjCollider(col);
				//}
				//delete components[idx]; ??
				components.DeleteAt(idx);
				break;
			}
		}
	}

private:
	List<std::shared_ptr<BaseComponent>> components{};
	List<std::shared_ptr<BaseComponent>> newComponents{};
	bool prevEnabled{};

	//void Copy(GameObject other, bool parent = true);

	template <typename T>
	T* AddComponentCopy(T* other) {
		GameObject* ptr{ this };
		std::shared_ptr<T> comp = std::make_shared<T>(new T(ptr, typeid(T).name()));
		
		//T* comp{ new T(ptr, typeid(T).name()) };
		components.Add(comp);
		newComponents.Add(comp);
		return comp.get();
	}

};


