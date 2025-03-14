#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "PlayerCommand.h"
#include "SceneManager.h"
#include "Scene.h"

void Player::Awake()
{
	id = SceneManager::GetInstance().curScene->FindObjCount([this](GameObject* obj) {return obj->GetComponent<Player>() != nullptr; }) -1;
	if (id == 0) {
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, SDLK_d, glm::vec3(40, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, SDLK_a, glm::vec3(-40, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, SDLK_w, glm::vec3(0, -40, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, SDLK_s, glm::vec3(0, 40, 0)));
	}
	else if (id == 1){
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, GamePad::DPad_Right, glm::vec3(40, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, GamePad::DPad_Left, glm::vec3(-40, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, GamePad::DPad_Up, glm::vec3(0, -40, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerCommand(gameObject, GamePad::DPad_Down, glm::vec3(0, 40, 0)));
	}

}

void Player::Update()
{
}