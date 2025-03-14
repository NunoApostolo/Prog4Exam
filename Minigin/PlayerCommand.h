#pragma once
#include "Command.h"

class PlayerCommand : public Command
{
public:
	PlayerCommand(GameObject* gameobject, GamePad btn, glm::vec3 moveVector);
	PlayerCommand(GameObject* gameobject, SDL_Keycode keyCode, glm::vec3 moveVector);

	void ExecutePressed() override;

private:
	glm::vec3 moveVec{};
};

