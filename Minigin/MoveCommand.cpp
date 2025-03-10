#include "MoveCommand.h"
#include "Transform.h"

MoveCommand::MoveCommand(GameObject* gameobject, SDL_Keycode keyCode, glm::vec3 moveVector)
	: Command(gameobject, keyCode)
{
	moveVec = moveVector;
}

void MoveCommand::ExecutePressed()
{
	gameObj->transform->SetPosition(gameObj->transform->position + moveVec * Time::deltaTime);
}
