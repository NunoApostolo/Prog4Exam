#pragma once
#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerUI.h"
#include "ResourceManager.h"
#include "EventTypes.h"
#include "GameManager.h"

#include "Console.h"

void Player::Awake()
{
}

void Player::Start()
{
	//id = SceneManager::GetInstance().curScene->FindObjCount([this](GameObject* obj) {return obj->GetComponent<Player>() != nullptr; }) - 1;
	//tex = gameObject->AddComponent<TextureRenderer>();
}

void Player::Init(int playerId, int phealth, int plives)
{
	this->id = playerId;
	this->health = phealth;
	this->lives = plives;

	tex = GameObject::Create("Tank Texture")->AddComponent<TextureRenderer>();
	tex->gameObject->SetParent(gameObject);
	tex->SetTexture("Player.png", Vector2(0.5f, 0.5f), 5);
	texSize = tex->GetSize().x / 2;
	barrelTex = GameObject::Create("Player Barrel")->AddComponent<TextureRenderer>();
	barrelTex->gameObject->SetParent(gameObject);
	barrelTex->SetTexture("TankBarrel.png", Vector2(0.2f, 0.5f), 6);

	gameObject->transform->localScale = Vector2(1.3f, 1.3f);

	if (id == 0)
	{
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_d, Vector3(speed, 0, 0), Direction::Right));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_a, Vector3(-speed, 0, 0), Direction::Left));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_w, Vector3(0, -speed, 0), Direction::Up));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_s, Vector3(0, speed, 0), Direction::Down));
		InputManager::GetInstance().RegisterCommand(new BarrelCommand(gameObject, SDLK_LEFT, true));
		InputManager::GetInstance().RegisterCommand(new BarrelCommand(gameObject, SDLK_RIGHT, false));
		InputManager::GetInstance().RegisterCommand(new ShootCommand(gameObject, SDLK_SPACE, 1));
		if (GameManager::GetInstance().GetMode() == GameMode::Solo) {
			InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Right, Vector3(speed, 0, 0), Direction::Right));
			InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Left, Vector3(-speed, 0, 0), Direction::Left));
			InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Up, Vector3(0, -speed, 0), Direction::Up));
			InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Down, Vector3(0, speed, 0), Direction::Down));
			InputManager::GetInstance().RegisterCommand(new BarrelCommand(gameObject, GamePad::None, true));
			InputManager::GetInstance().RegisterCommand(new ShootCommand(gameObject, GamePad::Right_Trigger, 1));
		}
	}
	else if (id == 1)
	{
		tex->SetTexture("Player2.png");
		barrelTex->SetTexture("TankBarrel2.png");
		//
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Right, Vector3(speed, 0, 0), Direction::Right));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Left, Vector3(-speed, 0, 0), Direction::Left));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Up, Vector3(0, -speed, 0), Direction::Up));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Down, Vector3(0, speed, 0), Direction::Down));
		InputManager::GetInstance().RegisterCommand(new BarrelCommand(gameObject, GamePad::None, true));
		InputManager::GetInstance().RegisterCommand(new ShootCommand(gameObject, GamePad::Right_Trigger, 1));
		//InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::A, 10));
	}

	//should this go somewhere else?
	subject = std::make_unique<Subject>();
	ui = GameObject::Create(std::string("PlayerUI" + id));

	if (id == 0)
	{
		ui->transform->position = Vector2(-200, -350);
	}
	else if (id == 1)
	{
		ui->transform->position = Vector2(200, -350);
	}

	PlayerUI* obs = ui->AddComponent<PlayerUI>();
	obs->Init(this);
	subject->RegisterObserver(obs);

	EventManager::GetInstance().AddListener<EventType>(this->gameObject, this, EventType::Score_Increase);
	EventManager::GetInstance().AddListener<EventType>(obs->gameObject, obs, EventType::Score_Change);
	EventManager::GetInstance().AddListener<EventType>(obs->gameObject, obs, EventType::Player_Hurt);
}

Player::~Player()
{
	EventManager::GetInstance().RemoveListener(gameObject);
	EventManager::GetInstance().RemoveListener(ui);
	GameObject::Delete(ui);
}

void Player::Spawn()
{
	gameObject->SetActive(true);
	isDead = false;
	health = 1;
}

void Player::Update()
{
	GameManager::GetInstance().CheckDiamond(gameObject);
}

void Player::TurnBarrelLeft()
{
	barrelTex->gameObject->transform->localRotation -= BARREL_ROT_SPEED * Time::deltaTime;
	if (barrelTex->gameObject->transform->localRotation < 0) barrelTex->gameObject->transform->localRotation += 360;
}

void Player::TurnBarrelRight()
{
	barrelTex->gameObject->transform->localRotation += BARREL_ROT_SPEED * Time::deltaTime;
	if (barrelTex->gameObject->transform->localRotation > 360) barrelTex->gameObject->transform->localRotation -= 360;
}

void Player::SetBarrelDir(const Vector2& barrelDir)
{
	float rot = Vector2().AngleDeg(Vector2(), barrelDir);
	Console::GetInstance().Log(std::to_string(rot));
	barrelTex->gameObject->transform->localRotation = rot;
}

void Player::SetDirection(Direction direction)
{
	dir = direction;
	switch (dir)
	{
	case Direction::Right:
		tex->gameObject->transform->localRotation = 0;
		break;
	case Direction::Up:
		tex->gameObject->transform->localRotation = 90;
		break;
	case Direction::Left:
		tex->gameObject->transform->localRotation = 180;
		break;
	case Direction::Down:
		tex->gameObject->transform->localRotation = 270;
		break;
	default:
		break;
	}
}

bool Player::CheckCollision(const Vector3& pos, const float unitCol)
{
	Vector3 selfPos{ gameObject->transform->GetPosition() };
	if (pos.x + unitCol >= selfPos.x - texSize && pos.x - unitCol <= selfPos.x + texSize
		&& pos.y + unitCol >= selfPos.y - texSize && pos.y - unitCol <= selfPos.y + texSize) {
		return true;
	}
	return false;
}

void Player::Shoot()
{
	if (IsDead()) return;
	Bullet* bullet = GameObject::Create("Player Bullet")->AddComponent<Bullet>();
	bullet->Init(gameObject->transform->GetPosition(), barrelTex->gameObject->transform->localRotation, "PlayerShell.png");
	bullet->SetOwner(this);
}

void Player::IncreaseScore(int scoreInc)
{
	score += scoreInc;
	//subject->Notify(EventType::Score_Change, EventArgs({id, score}));
	EventManager::GetInstance().SendMessage<EventType>(EventType::Score_Change, EventArgs({ uint16_t(GetId()), score }));
}

void Player::TakeDamage(int dmg)
{
	if (!isDead) 
	{
		health -= dmg;
		//subject->Notify(Event::Player_Hurt, EventArgs({id, health}));
		if (health <= 0) {
			Die();
		}
	}
}

void Player::Die()
{
	isDead = true;

	lives--;
	gameObject->SetActive(false);
	GameManager::GetInstance().NotifyPlayerDeath(this);
	EventManager::GetInstance().SendMessage<EventType>(EventType::Player_Hurt, EventArgs({ uint16_t(GetId()), lives }));
	//subject->Notify(Event::Player_Die, EventArgs({id}));
}

void Player::HandleEvent(Event, EventArgs args)
{
	//...
	if (args[0] == id) IncreaseScore(args[1]);
}
PlayerMoveCommand::PlayerMoveCommand(GameObject* gameobject, GamePad button, Vector3 moveVector, Direction direction)
	: Command(gameobject, button)
{
	moveVec = moveVector;
	dir = direction;
	player = gameObj->GetComponent<Player>();
}

PlayerMoveCommand::PlayerMoveCommand(GameObject* gameobject, SDL_Keycode keyCode, Vector3 moveVector, Direction direction)
	: Command(gameobject, keyCode)
{
	moveVec = moveVector;
	dir = direction;
	player = gameObj->GetComponent<Player>();
}

void PlayerMoveCommand::ExecuteDown()
{
	player->SetDirection(dir);
}

void PlayerMoveCommand::ExecutePressed()
{
	//ServiceLocator::GetAudioService().PlayAudio("Danggo_Lv2.wav");
	if (player->dir == Direction::None) ExecuteDown();
	if (player->dir != dir) return;
	Vector3 prevPos = gameObj->transform->position;
	gameObj->transform->position = (gameObj->transform->position + moveVec * Time::deltaTime);
	if (GameManager::GetInstance().CheckColliders(gameObj->transform->GetPosition(), player->texSize)) {
		gameObj->transform->position = prevPos;
	}
}

void PlayerMoveCommand::ExecuteUp()
{
	player->dir = Direction::None;
}

void ShootCommand::ExecuteDown()
{
	player->Shoot();
}

void BarrelCommand::ExecutePressed()
{
	if (isLeft) {
		player->TurnBarrelLeft();
	}
	else {
		player->TurnBarrelRight();
	}
}

void BarrelCommand::MoveRThumb(const Vector2& thumb)
{
	player->SetBarrelDir(thumb);
}
