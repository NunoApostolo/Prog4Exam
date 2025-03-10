#include "MovementTest.h"
#include "GameObject.h"
#include "Transform.h"
#include "MoveCommand.h"

void MovementTest::Start()
{
	InputManager::GetInstance().RegisterCommand(new MoveCommand(gameObject, SDLK_d, glm::vec3(40, 0, 0)));
	InputManager::GetInstance().RegisterCommand(new MoveCommand(gameObject, SDLK_a, glm::vec3(-40, 0, 0)));
	InputManager::GetInstance().RegisterCommand(new MoveCommand(gameObject, SDLK_w, glm::vec3(0, -40, 0)));
	InputManager::GetInstance().RegisterCommand(new MoveCommand(gameObject, SDLK_s, glm::vec3(0, 40, 0)));
}

void MovementTest::Update()
{
}