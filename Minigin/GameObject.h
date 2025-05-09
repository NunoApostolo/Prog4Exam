#pragma once
#include <string>
#include <memory>
//#include "List.h"
#include "BaseComponent.h"
//#include <utils.h>
#include "TimeManager.h"
//#include "Camera.h"
//#include "ColliderManager.h"
#include <glm.hpp>
//#include "Transform.h"
#include <iostream>
#include "IMouseHandler.h"

class Transform;
class Scene;
class GameObject final
{
	friend class Scene;
public:
	std::string name;
	Transform* transform;

	GameObject* parentPtr;
	std::vector<GameObject*> childrenPtr; // framework won't be used much

	static GameObject* Create(std::string name);
	static bool Delete(GameObject* obj);

	GameObject(std::string name, const Vector3& pos, const Vector3& scale, float rotarion, GameObject* parent = nullptr); // just in case, different constructor behaviour
	GameObject(std::string name);
	GameObject();
	GameObject& operator =(const GameObject&) = delete;
	GameObject operator =(GameObject) = delete;
	~GameObject();
	void Init();

	void Update(); // updates components
	void Render();// draws components
	void Render(int order);// draws components
	void RenderUI();

	//GameObject* FindChild(std::string cname);

	void SetParent(GameObject* parent);

	template <typename T> // dynamic? / not seperating template code from header. It's a pain otherwise.
	T* AddComponent() {
		static_assert(std::is_base_of<BaseComponent, T>::value, "Must be BaseComponent");

		GameObject* ptr{ this };
		T* comp{ new T() };
		dynamic_cast<BaseComponent*>(comp)->Initialize(ptr, typeid(T).name());
		//T* comp{ new T(ptr, typeid(T).name()) };
		//std::shared_ptr<T> comp(new T(ptr, typeid(T).name()));

		components.emplace_back(std::move(std::unique_ptr<T>(comp)));
		newComponents.emplace_back(comp);
		comp->Awake();

		if (dynamic_cast<IMouseHandler*>(comp)) {
			dynamic_cast<IMouseHandler*>(comp)->Init(ptr);
		}

		return comp;
	}
	template <typename T>
	T* GetComponent() {

		for (size_t idx{}; idx < components.size(); idx++) {
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
	std::vector<T*> GetAllComponents() {
		std::vector<T*> list{};
		for (int idx{}; idx < components.size(); idx++) {
			if (components[idx]->GetType() == typeid(T).name()) {
				list.emplace_back((T*)components[idx].get());
			}
		}
		return list;
	}

	std::vector<BaseComponent*> GetAllComponents() {
		std::vector<BaseComponent*> comps{};
		for (auto& comp : components) {
			comps.emplace_back(comp.get());
		}
		return comps;
	}
	template <typename T>
	T* GetComponentInChildren() {
		for (int cidx{}; cidx < childrenPtr.size(); cidx++) {
			for (int idx{}; idx < childrenPtr[cidx]->components.size(); idx++) {
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
		for (int idx{}; idx < components.size(); idx++) {
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
		//int index{ 0 };
		//for (auto& c : components) {
		//	if (c.get)
		//}
		components.erase(std::find_if(components.begin(), components.end(), [&](const std::unique_ptr<BaseComponent>& comp) {
			bool check{ comp->GetType() == typeid(T).name() };
			if (check) {
				EventManager::GetInstance().RemoveListener(comp.get()->gameObject);
			}
			return check; 
			}));
		
	}

	void SetActive(bool active);
	bool IsActive() { return enabled; }
	int GetOrder() { return order; }
	void SetOrder(int renderOrder) { order = renderOrder; }
private:
	void SetInternActive(bool active);
	bool GetActive();

	std::vector<std::unique_ptr<BaseComponent>> components{};
	std::vector<BaseComponent*> newComponents{};
	bool enabled{ true };
	bool localEnabled{ true };
	bool prevEnabled{};
	int order{ 0 };
	bool deleted{ false };

	static std::vector<std::unique_ptr<GameObject>> objToCreate;
	static std::vector<GameObject*> objToDelete;

	//void Copy(GameObject other, bool parent = true);

	//template <typename T>
	//T* AddComponentCopy(T* other) {
	//	GameObject* ptr{ this };
	//	T* comp = new T(ptr, typeid(T).name());
	//	
	//	//T* comp{ new T(ptr, typeid(T).name()) };
	//	components.emplace_back(std::unique_ptr<T>(comp));
	//	newComponents.emplace_back(comp);
	//	return comp;
	//}
protected:
	static void CreateObjects(Scene* curScene);
	static void DeleteObjects(Scene* curScene);

};


