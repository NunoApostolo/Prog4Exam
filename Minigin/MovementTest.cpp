#include "MovementTest.h"
#include "GameObject.h"
#include "Transform.h"

void MovementTest::Start()
{
	InputManager::GetInstance().RegisterCommand(static_cast<SDL_Keycode>(SDLK_a), [this]() { this->Move(SDLK_a); });
	InputManager::GetInstance().RegisterCommand(static_cast<SDL_Keycode>(SDLK_d), [this]() { this->Move(SDLK_d); });
	InputManager::GetInstance().RegisterCommand(static_cast<SDL_Keycode>(SDLK_w), [this]() { this->Move(SDLK_w); });
	InputManager::GetInstance().RegisterCommand(static_cast<SDL_Keycode>(SDLK_s), [this]() { this->Move(SDLK_s); });
}

void MovementTest::Update()
{
}

void MovementTest::Move(SDL_Keycode key)
{
	const int speed{ 40 };
	if (key == (SDLK_d)) {
		gameObject->transform->localPosition.x += speed * Time::deltaTime;
	}
	if (key == (SDLK_a)) {
		gameObject->transform->localPosition.x += speed * -1 * Time::deltaTime;
	}
	if (key == (SDLK_s)) {
		gameObject->transform->localPosition.y += speed * Time::deltaTime;
	}
	if (key == (SDLK_w)) {
		gameObject->transform->localPosition.y += speed * -1 * Time::deltaTime;
	}
}
