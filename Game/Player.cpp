#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerUI.h"
#include "ResourceManager.h"
#include "EventTypes.h"
#include "GameManager.h"

void Player::Awake()
{
}

void Player::Start()
{
	//id = SceneManager::GetInstance().curScene->FindObjCount([this](GameObject* obj) {return obj->GetComponent<Player>() != nullptr; }) - 1;
	//tex = gameObject->AddComponent<TextureRenderer>();
	tex = GameObject::Create("Tank Texture")->AddComponent<TextureRenderer>();
	tex->gameObject->SetParent(gameObject);
	tex->SetTexture("Player.png", Vector2(0.5f,0.5f), 5);
	texSize = tex->GetSize().x/2;
	barrelTex = GameObject::Create("Player Barrel")->AddComponent<TextureRenderer>();
	barrelTex->gameObject->SetParent(gameObject);
	barrelTex->SetTexture("TankBarrel.png", Vector2(0.2f, 0.5f), 6);

	gameObject->transform->localScale = Vector2(1.3f,1.3f);

	if (id == 0) 
	{
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_d, Vector3(speed, 0, 0), Direction::Right));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_a, Vector3(-speed, 0, 0), Direction::Left));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_w, Vector3(0,-speed, 0), Direction::Up));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_s, Vector3(0, speed, 0), Direction::Down));
		InputManager::GetInstance().RegisterCommand(new ShootCommand(gameObject, SDLK_SPACE, 1));
		InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_z, 10));
	}
	else if (id == 1) 
	{
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Right, Vector3(speed, 0, 0), Direction::Right));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Left, Vector3(-speed, 0, 0), Direction::Left));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Up, Vector3(0, -speed, 0), Direction::Up));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Down, Vector3(0, speed, 0), Direction::Down));
		InputManager::GetInstance().RegisterCommand(new ShootCommand(gameObject, GamePad::X, 1));
		InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::A, 10));
	}

	health = 3;

	//should this go somewhere else?
	subject = std::make_unique<Subject>();
	TextObject* text = GameObject::Create(std::string("PlayerUI" + id))->AddComponent<TextObject>();
	text->SetText("#lives: " + std::to_string(health) + std::to_string(score), ResourceManager::GetInstance().LoadFont("Lingua.otf", 30));

	if (id == 0) 
	{
		text->gameObject->transform->position = Vector3(5, 100, 0);
	}
	else if (id == 1) 
	{
		text->gameObject->transform->position = Vector3(5, 180, 0);
	}

	PlayerUI* obs = text->gameObject->AddComponent<PlayerUI>();
	obs->Init(this);
	subject->RegisterObserver(obs);

	EventManager::GetInstance().AddListener<EventType>(this->gameObject, this, EventType::Score_Increase);
	EventManager::GetInstance().AddListener<EventType>(obs->gameObject, obs, EventType::Score_Change);
	EventManager::GetInstance().AddListener<EventType>(obs->gameObject, obs, EventType::Player_Hurt);
}

void Player::Update()
{
	if (InputManager::GetInstance().KeyPressed(SDLK_LEFT)) 
	{
		TurnBarrelLeft();
	}
	if (InputManager::GetInstance().KeyPressed(SDLK_RIGHT)) 
	{
		TurnBarrelRight();
	}
	//temp
	//if (InputManager::GetInstance().KeyDown(SDLK_c)) {
	//	TakeDamage(1);
	//}
	//if (InputManager::GetInstance().KeyDown(SDLK_z)) {
	//	IncreaseScore(10);
	//}
}

void Player::Init(int playerId, int phealth, int plives)
{
	this->id = playerId;
	this->health = phealth;
	this->lives = plives;
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

void Player::Shoot()
{
	Bullet* bullet = GameObject::Create("Player Bullet")->AddComponent<Bullet>();
	bullet->Init(gameObject->transform->GetPosition(), barrelTex->gameObject->transform->localRotation, "PlayerShell.png");
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
		EventManager::GetInstance().SendMessage<EventType>(EventType::Player_Hurt, EventArgs({ uint16_t(GetId()), health }));
		if (health <= 0) {
			Die();
		}
	}
}

void Player::Die()
{
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

void ScoreCommand::ExecuteDown()
{
	// temp, example: in other class to send depending on which player did something
	EventManager::GetInstance().SendMessage<EventType>(EventType::Score_Increase, EventArgs({uint16_t(player->GetId()), 100}));
	//player->IncreaseScore(score);
}
