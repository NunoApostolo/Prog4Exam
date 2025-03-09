#include "BaseComponent.h"
#include "InputManager.h"

#pragma once
class MovementTest : public BaseComponent
{
public:
	MovementTest(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	void Start() override;
	void Update() override;

	void Move(SDL_Keycode key);

private:

};

