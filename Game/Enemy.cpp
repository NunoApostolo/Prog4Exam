#include "Enemy.h"
#include "InputManager.h"
#include "GameManager.h"
#include "utils.h"
#include "Console.h"
#include "Random.h"
#include "ServiceLocator.h"

std::unique_ptr<IdleState> BaseEnemyState::idle{ std::unique_ptr<IdleState>(new IdleState())};
std::unique_ptr<WanderState> BaseEnemyState::wandering{ std::unique_ptr<WanderState>(new WanderState())};
std::unique_ptr<ChaseState> BaseEnemyState::chasing{ std::unique_ptr<ChaseState>(new ChaseState())};

void IdleState::Update(Enemy* owner)
{
	owner->CreatePath(GameManager::GetInstance().GetRandomPos());
	owner->SetState(wandering.get());
}

void WanderState::Update(Enemy* owner)
{
	owner->UpdateMovePath();
	if (owner->GetPathCount() == 0) {
		owner->SetState(idle.get());
	}
	if (Vector3::Distance(owner->gameObject->transform->position, GameManager::GetInstance().GetPlayerPos(owner->gameObject->transform->GetPosition())) <= 200) {
		owner->ClearPath();
		owner->SetState(chasing.get());
	}
}

void ChaseState::Update(Enemy* owner)
{
	owner->UpdateMovePath();
	if (owner->GetPathCount() <= 1) {
		owner->CreatePath(GameManager::GetInstance().GetPlayerPos(owner->gameObject->transform->GetPosition()));
	}

	if (shootTimer > 0) {
		shootTimer -= Time::deltaTime;
	}
	else {
		owner->Shoot();
		shootTimer = 2.f;
	}

	Player* pl = GameManager::GetInstance().CheckPlayerColliders(owner->gameObject->transform->position, owner->tankTex->GetSize().x / 2);
	if (pl != nullptr) {
		pl->TakeDamage(3);
	}

	if (Vector3::Distance(owner->gameObject->transform->position, GameManager::GetInstance().GetPlayerPos(owner->gameObject->transform->GetPosition())) > 400) {
		owner->SetState(idle.get());
	}
}

void Enemy::Start()
{
	SetState(BaseEnemyState::idle.get());

	gameObject->transform->localScale = Vector2(1.3f,1.3f);
	tankTex = GameObject::Create("Enemy Texture")->AddComponent<TextureRenderer>();
	tankTex->SetTexture("Tank.png", Vector2(0.5f, 0.5f), 5);
	tankTex->gameObject->SetParent(gameObject);
	tankTex->gameObject->transform->localPosition = Vector2();
	texSize = tankTex->GetSize().x / 2;

	Init();
}

void Enemy::Update()
{
	curState->Update(this);
}

void Enemy::UpdateMovePath()
{
	if (path.size() != 0) {
		Vector3 moveDir{ (path[0] - gameObject->transform->position).Normalize()};
		gameObject->transform->position += moveDir * speed * Time::deltaTime;

		if (Vector3().Distance(path[0], gameObject->transform->GetPosition()) <= 2) {
			gameObject->transform->position = path[0];
			path.erase(path.begin());

			SetDirection();
		}

		if (GameManager::GetInstance().CheckColliders(gameObject->transform->position, tankTex->GetSize().x / 2)) {
			path.clear();
			int x{}, y{};
			GameManager::GetInstance().GetGridPos(gameObject->transform->position,x,y);
			gameObject->transform->position = GameManager::GetInstance().GetPosInGrid(x, y);
		}
	}
}
void Enemy::SetDirection()
{
	// could just use the angle for rotation?
	if (path.size() != 0) {
		float angle{ Vector3().AngleDeg(gameObject->transform->position, path[0]) };
		if (angle <= 45 || angle >= 315) dir = Direction::Right;
		else if (angle <= 135) dir = Direction::Down;
		else if (angle <= 225) dir = Direction::Left;
		else if (angle <= 315) dir = Direction::Up;
	}
	switch (dir)
	{
	case Direction::Right:
		tankTex->gameObject->transform->localRotation = 0;
		break;
	case Direction::Down:
		tankTex->gameObject->transform->localRotation = 90;
		break;
	case Direction::Left:
		tankTex->gameObject->transform->localRotation = 180;
		break;
	case Direction::Up:
		tankTex->gameObject->transform->localRotation = 270;
		break;
	default:
		break;
	}
}

void Enemy::SetState(BaseEnemyState* state)
{
	curState = state;
}

void Enemy::CreatePath(const Vector3& dest)
{
	ClearPath();
	if (dest == gameObject->transform->GetPosition()) return;

	int x{}, y{};
	GameManager::GetInstance().GetGridPos(dest, x, y);
	Vector3 gridDest = GameManager::GetInstance().GetPosInGrid(x, y);

	//Console::GetInstance().Log("destination: " + gridDest);
	//Console::GetInstance().Log("destination(x,y): " + std::to_string(x) + " " + std::to_string(y));

	Vector3 peekPos{ gameObject->transform->position };
	Vector3 diff{ gridDest - peekPos };
	Direction prevDir{ Direction::None };
	Direction peekDir{Direction::None};

	if (std::abs(diff.x) >= std::abs(diff.y)) {

		if (diff.x >= 0) peekDir = Direction::Right;
		else peekDir = Direction::Left;
	}
	else {
		if (diff.y >= 0) peekDir = Direction::Down;
		else peekDir = Direction::Up;
	}
	prevDir = peekDir;

	int helpCounter{ 0 };

	// this sucks
	while (true) {
		//&& (peekDir != Direction::Left && peekDir != Direction::Right) || 
		//(peekDir == Direction::Up || peekDir == Direction::Down)
		prevDir = peekDir;
		GameManager::GetInstance().GetGridPos(peekPos, x, y);

		if (GameManager::GetInstance().IsPathInDirectionFree(peekPos, peekDir)) {
			switch (peekDir)
			{
			case Direction::Right:
				++x;
				break;
			case Direction::Up:
				--y;
				break;
			case Direction::Left:
				--x;
				break;
			case Direction::Down:
				++y;
				break;
			default:
				break;
			}
			peekPos = GameManager::GetInstance().GetPosInGrid(x, y);
			if (Vector3().Distance(peekPos, gridDest) <= 20) peekPos = gridDest;
			
			diff = gridDest - peekPos;
			if (std::abs(diff.x) >= std::abs(diff.y)) {

				if (diff.x >= 0) peekDir = Direction::Right;
				else peekDir = Direction::Left;
			}
			else {
				if (diff.y >= 0) peekDir = Direction::Down;
				else peekDir = Direction::Up;
			}

			if (peekDir == utils::OffsetEnum<Direction>(prevDir, Direction::Down, Direction::None, 2)) {
				peekDir = utils::OffsetEnum<Direction>(prevDir, Direction::Down, Direction::None, 1);
				GameManager::GetInstance().GetGridPos(peekPos, x, y);
				if (!GameManager::GetInstance().IsPathInDirectionFree(peekPos, peekDir)) {
					peekDir = utils::OffsetEnum<Direction>(peekDir, Direction::Down, Direction::None, -2);
				}
			}
			if (!GameManager::GetInstance().IsPathInDirectionFree(peekPos, peekDir)) {
				if (!GameManager::GetInstance().IsPathInDirectionFree(peekPos, prevDir)) {
					peekDir = utils::OffsetEnum<Direction>(prevDir, Direction::Down, Direction::None, 2);
				}
				else {
					peekDir = prevDir;
				}
			}
			// gambling
			if (helpCounter >= 100) {
				peekDir = static_cast<Direction>(Random::GetRandom(1, 4));
				prevDir = peekDir;
				//helpCounter = 0;
				CreatePath(GameManager::GetInstance().GetRandomPos());
				return;
			}
		}
		else {
			peekDir = utils::OffsetEnum<Direction>(peekDir, Direction::Down, Direction::None, 1);
			GameManager::GetInstance().GetGridPos(peekPos, x, y);
			if (!GameManager::GetInstance().IsPathInDirectionFree(peekPos, peekDir)) {
				peekDir = utils::OffsetEnum<Direction>(peekDir, Direction::Down, Direction::None, -3);
			}
			prevDir = peekDir;
		}

		if (prevDir != peekDir || peekPos == gridDest) {
			GameManager::GetInstance().GetGridPos(peekPos, x, y);
			//Console::GetInstance().Log("(x,y)" + std::to_string(x) + " " + std::to_string(y));

			if (std::find(path.begin(), path.end(), peekPos) != path.end()) {
				//peekDir = utils::OffsetEnum<Direction>(peekDir, Direction::Down, Direction::None, 1);
				//GameManager::GetInstance().GetGridPos(peekPos, x, y);
				//if (!GameManager::GetInstance().IsPathInDirectionFree(peekPos, peekDir)) {
				//	peekDir = utils::OffsetEnum<Direction>(peekDir, Direction::Down, Direction::None, -2);
				//}
				//prevDir = peekDir;
				path.emplace_back(peekPos);
			}
			else {
				path.emplace_back(peekPos);
			}
		}
		if (path.size() != 0) {
			if (path[path.size() - 1] == gridDest) {

				//for (auto& p : path) {
				//	TextureRenderer* testT = GameObject::Create("test")->AddComponent<TextureRenderer>();
				//	testT->gameObject->transform->position = p;
				//	testT->gameObject->transform->localScale = Vector2(0.1f,0.1f);
				//	testT->SetTexture("", Vector2(0.5f, 0.5f), 1000);
				//}

				break;
			}
		}
		helpCounter++;
	}

	SetDirection();
}

void Enemy::ClearPath()
{
	path.clear();
}

int Enemy::GetPathCount()
{
	return static_cast<int>(path.size());
}

void TankEnemy::Init()
{
	tankTex->SetTexture("Tank.png");
}

void TankEnemy::Shoot()
{
	Bullet* bullet = GameObject::Create("Enemy Bullet")->AddComponent<Bullet>();
	bullet->Init(gameObject->transform->GetPosition(), tankTex->gameObject->transform->localRotation, "EnemyShell.png");
	bullet->SetOwner(this);
}

void Enemy::TakeDamage(int dmg, Player* player)
{
	player;
	hp -= dmg;

	if (hp <= 0) {
		ServiceLocator::GetAudioService().PlayAudio("explosion.wav", 0.5f);
		player->IncreaseScore(100);

		GameManager::GetInstance().NotifyEnemyDeath(this);

		GameObject::Delete(gameObject);
	}
}

bool Enemy::CheckCollision(const Vector3& pos, const float unitCol)
{
	Vector3 selfPos{ gameObject->transform->GetPosition() };
	if (pos.x + unitCol >= selfPos.x - texSize && pos.x - unitCol <= selfPos.x + texSize
		&& pos.y + unitCol >= selfPos.y - texSize && pos.y - unitCol <= selfPos.y + texSize) {
		return true;
	}
	return false;
}

void MeleeEnemy::Init()
{
	tankTex->SetTexture("Recognizer.png");
	speed = 150;
}
