#pragma once
#include "SDL.h"
#include "GameObject.h"

class Command
{
public:
	Command(GameObject* gameobject, SDL_Keycode keyCode);

	virtual void ExecuteDown() {}
	virtual void ExecutePressed() {}
	virtual void ExecuteUp() {}

	SDL_Keycode GetKey();
protected:
	SDL_Keycode key{};
	GameObject* gameObj{};
};

