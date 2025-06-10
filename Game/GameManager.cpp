#include "GameManager.h"
#include "InputManager.h"
#include "ServiceLocator.h"
#include "Camera.h"
#include "Console.h"
#include "Random.h"

GameManager* GameManager::instance{};

void GameManager::Init()
{
	instance = this;

	Random::SetRandomSeed();

	mapSize = static_cast<float>(Camera::GetMainCamera().GetViewPort().width);
	player = GameObject::Create("Player")->AddComponent<Player>();

	mapCreator.Init();

	mapCreator.LoadMap("Map1.data", mapSize);

	for (int y{1}; y < GRID_SIZE; y++) {
		for (int x{1}; x < GRID_SIZE; x++) {
			if (!CheckColliders(GetPosInGridInternal(x, y), 5)) {
				freePosList.emplace_back(GetPosInGridInternal(x, y));
			}
		}
	}

	Enemy* testEnemy = GameObject::Create("test")->AddComponent<Enemy>();
	testEnemy->gameObject->transform->position = Vector2(-mapSize / 2, (-mapSize + TOP_OFFSET) / 2) + 
		Vector2(mapSize / GRID_SIZE * 17, mapSize / GRID_SIZE * 1);
	enemies.emplace_back(testEnemy);
}

void GameManager::Update()
{
	if (InputManager::GetInstance().KeyDown(SDLK_a)) {
		//ServiceLocator::GetAudioService().PlayAudio("Danggo_Lv2.wav", 0.5f);
	}
}

Vector3 GameManager::GetPosInGridInternal(int x, int y)
{
	return Vector2(-mapSize / 2, (-mapSize + TOP_OFFSET) / 2) + Vector3(mapSize / GRID_SIZE * x, mapSize / GRID_SIZE * y, 0);
}

Vector3 GameManager::GetPosInGrid(int x, int y)
{
	Vector3 pos = GetPosInGridInternal(x,y);
	
	int index{ 0 }, closestIndex{ 0 };
	for (auto& p : freePosList) {
		if (Vector3::Distance(pos, p) <= Vector3::Distance(pos, freePosList[closestIndex])) {
			closestIndex = index;
		}
		++index;
	}
	return freePosList[closestIndex];
}

void GameManager::GetGridPos(const Vector3& pos, int& x, int& y)
{
	x = static_cast<int>(std::ceilf((pos.x + (mapSize) / 2) / (mapSize / 18)));
	y = static_cast<int>(std::ceilf((pos.y + (mapSize-TOP_OFFSET) / 2) / (mapSize / 18)));
}

bool GameManager::IsPathInDirectionFree(const Vector3& pos, const Direction dir)
{
	switch (dir)
	{
	case Direction::Right:
		return !CheckColliders(pos + Vector3(mapSize / GRID_SIZE, 0, 0), 5);
		break;
	case Direction::Up:
		return !CheckColliders(pos - Vector3(0, mapSize / GRID_SIZE, 0), 5);
		break;
	case Direction::Left:
		return !CheckColliders(pos - Vector3(mapSize / GRID_SIZE, 0, 0), 5);
		break;
	case Direction::Down:
		return !CheckColliders(pos + Vector3(0, mapSize / GRID_SIZE, 0), 5);
		break;
	default:
		Console::GetInstance().Log("Get Path went wrong: " + pos);
		return false;
		break;
	}
}

Vector3 GameManager::GetRandomPos()
{
	return freePosList[Random::GetRandom(0, freePosList.size())];
}

Vector3 GameManager::GetPlayerPos()
{
	return player->gameObject->transform->position;
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
	CollisionDirection dir{};
	for (auto& b : blocks) {
		if (b->CheckCollision(pos, unitCol, dir)) return true;
	}
	return false;
}

bool GameManager::CheckColliders(const Vector3& pos, const float unitCol, CollisionDirection& dir)
{
	for (auto& b : blocks) {
		if (b->CheckCollision(pos, unitCol, dir)) return true;
	}
	return false;
}

Player* GameManager::CheckPlayerColliders(const Vector3& pos, const float unitCol)
{
	if (player->CheckCollision(pos, unitCol)) return player;
	return nullptr;
}

Enemy* GameManager::CheckEnemyColliders(const Vector3& pos, const float unitCol)
{
	for (auto& e : enemies) {
		if (e->CheckCollision(pos, unitCol)) return e;
	}
	return nullptr;
}

void GameManager::NotifyEnemyDeath(Enemy* en)
{
	enemies.erase(std::find(enemies.begin(), enemies.end(), en));
}

