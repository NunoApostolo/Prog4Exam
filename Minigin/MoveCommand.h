#pragma once
#include "Command.h"

class MoveCommand : public Command
{
public:
	MoveCommand(GameObject* gameobject, SDL_Keycode keyCode, glm::vec3 moveVector);

	void ExecutePressed() override;

private:
	glm::vec3 moveVec{};
};

