#pragma once
#include "Command.h"

class PlayerMoveCommand : public Command
{
public:
	PlayerMoveCommand(GameObject* gameobject, GamePad btn, Vector3 moveVector);
	PlayerMoveCommand(GameObject* gameobject, SDL_Keycode keyCode, Vector3 moveVector);

	void ExecutePressed() override;

private:
	Vector3 moveVec{};
};

