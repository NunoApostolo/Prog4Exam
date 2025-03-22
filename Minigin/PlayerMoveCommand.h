#pragma once
#include "Command.h"

class PlayerMoveCommand : public Command
{
public:
	PlayerMoveCommand(GameObject* gameobject, GamePad btn, glm::vec3 moveVector);
	PlayerMoveCommand(GameObject* gameobject, SDL_Keycode keyCode, glm::vec3 moveVector);

	void ExecutePressed() override;

private:
	glm::vec3 moveVec{};
};

