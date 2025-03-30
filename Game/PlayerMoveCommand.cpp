#include "PlayerMoveCommand.h"
#include "Transform.h"

PlayerMoveCommand::PlayerMoveCommand(GameObject* gameobject, GamePad button, Vector3 moveVector)
	: Command(gameobject, button)
{
	moveVec = moveVector;
}

PlayerMoveCommand::PlayerMoveCommand(GameObject* gameobject, SDL_Keycode keyCode, Vector3 moveVector)
	: Command(gameobject, keyCode)
{
	moveVec = moveVector;
}

void PlayerMoveCommand::ExecutePressed()
{
	gameObj->transform->position = (gameObj->transform->position + moveVec * Time::deltaTime);
}
