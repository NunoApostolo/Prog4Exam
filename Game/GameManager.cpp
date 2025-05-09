#include "GameManager.h"
#include "InputManager.h"
#include "ServiceLocator.h"
#include "Camera.h"

void GameManager::Init()
{
	player = GameObject::Create("Player")->AddComponent<Player>();

	mapCreator.Init();

	mapCreator.LoadMap("Map1.data", static_cast<float>(Camera::GetMainCamera().GetViewPort().width));
}

void GameManager::Update()
{
	if (InputManager::GetInstance().KeyDown(SDLK_a)) {
		//ServiceLocator::GetAudioService().PlayAudio("Danggo_Lv2.wav", 0.5f);
	}
}

void GameManager::AddColliders(std::vector<MapBlock*> cols)
{
	blocks = cols;
}

void GameManager::ClearColliders()
{
	blocks.clear();
}

bool GameManager::CheckColliders(const Vector3& pos, const float unitCol)
{
	for (auto& b : blocks) {
		if (b->CheckCollision(pos, unitCol)) return true;
	}
	return false;
}

