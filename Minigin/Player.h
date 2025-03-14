#include "BaseComponent.h"
#include "InputManager.h"

#pragma once
class Player : public BaseComponent
{
public:
	Player(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	void Awake() override;
	void Update() override;

private:
	int id{ 0 };
};

