//#include <utils.h>
#include <functional>
#include "Dictionary.h"
#include <list>
#include <memory>
//#include "GameObject.h"

class GameObject;
class Collider;
//using pFunc = std::function<void()>;
#pragma once
class BaseComponent
{
public:
	bool enabled{ true };
	//allows to register functions
	Dictionary<std::string, std::function<void()>> funcMap{};

	BaseComponent(GameObject* objPtr, std::string type);
	BaseComponent() = delete;
	BaseComponent(const BaseComponent&) = delete;
	BaseComponent(BaseComponent&&) = delete;
	BaseComponent& operator =(const BaseComponent&) = delete;
	BaseComponent operator =(BaseComponent) = delete;
	
	virtual ~BaseComponent();
	
	virtual void Awake() {};
	virtual void Start() {};

	virtual void Update() {}; // empty
	virtual void FixedUpdate() {}; // empty
	virtual void Render() const {};
	virtual void RenderUI() {};

	virtual void OnDisable() {};
	virtual void OnEnable() {};

	template <class T, typename funcPtr>
	void RegisterFunction(std::string name, T* obj, funcPtr func) {
		std::function<void()> f = func;
	}
	std::function<void()> GetFunction(std::string name);

	std::string GetType();

	virtual void OnCollitionEnter(Collider*) {};
	virtual void OnCollition(Collider*) {};
	virtual void OnCollitionExit(Collider*) {};

	GameObject* gameObject; // need vars in object
	void CheckGO();
protected:
	std::string classType; // something malicious is brewing

private:
	GameObject* checkGO{};
};

