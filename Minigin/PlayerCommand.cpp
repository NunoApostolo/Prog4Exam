#include "PlayerCommand.h"
#include "Transform.h"

PlayerCommand::PlayerCommand(GameObject* gameobject, GamePad button, glm::vec3 moveVector)
	: Command(gameobject, button)
{
	moveVec = moveVector;
}

PlayerCommand::PlayerCommand(GameObject* gameobject, SDL_Keycode keyCode, glm::vec3 moveVector)
	: Command(gameobject, keyCode)
{
	moveVec = moveVector;
}

void PlayerCommand::ExecutePressed()
{
	gameObj->transform->position = (gameObj->transform->position + moveVec * Time::deltaTime);
}
