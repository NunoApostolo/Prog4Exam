#pragma once
#include "SDL.h"
#include "GameObject.h"

enum class GamePad {
	None = 0,
	DPad_Up = 0x001,
	DPad_Down = 0x0002,
	DPad_Left = 0x0004,
	DPad_Right = 0x0008,
	Start = 0x0010,
	Back = 0x0020,
	Left_Thumb = 0x0040,
	RightThumb = 0x0080,
	Left_Shoulder = 0x0100,
	Right_Shoulder = 0x0200,
	A = 0x1000,
	B = 0x2000,
	X = 0x4000,
	Y = 0x8000,
	Left_Trigger = 0x10000,
	Right_Trigger = 0x20000
};

class Command
{
public:
	Command(GameObject* gameobject, GamePad btn);
	Command(GameObject* gameobject, SDL_Keycode keyCode);

	virtual void ExecuteDown() {}
	virtual void ExecutePressed() {}
	virtual void ExecuteUp() {}
	virtual void MoveLThumb(const Vector2&) {}
	virtual void MoveRThumb(const Vector2&) {}

	SDL_Keycode GetKey();
	unsigned long GetBtn();
	GameObject* GetObj();
	int GetUser() { return gamePadUser; }
	bool isGamePad{ false };
protected:
	SDL_Keycode key{};
	unsigned long btn{};
	GameObject* gameObj{};
	int gamePadUser{ 0 };
};

