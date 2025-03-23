#include "Player.h"
#include "GameObject.h"
#include "Transform.h"
#include "PlayerMoveCommand.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PlayerUI.h"
#include "ResourceManager.h"

void Player::Awake()
{
}

void Player::Start()
{
	//id = SceneManager::GetInstance().curScene->FindObjCount([this](GameObject* obj) {return obj->GetComponent<Player>() != nullptr; }) - 1;
	if (id == 0) {
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_d, Vector3(speed, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_a, Vector3(-speed, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_w, Vector3(0,-speed, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, SDLK_s, Vector3(0, speed, 0)));
		InputManager::GetInstance().RegisterCommand(new HurtCommand(gameObject, SDLK_c, 1));
		InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_z, 10));
	}
	else if (id == 1) {
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Right, Vector3(speed, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Left, Vector3(-speed, 0, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Up, Vector3(0, -speed, 0)));
		InputManager::GetInstance().RegisterCommand(new PlayerMoveCommand(gameObject, GamePad::DPad_Down, Vector3(0, speed, 0)));
		InputManager::GetInstance().RegisterCommand(new HurtCommand(gameObject, GamePad::X, 1));
		InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::A, 10));
	}

	health = 3;

	//should this go somewhere else?
	subject = std::make_unique<Subject>();
	TextObject* text = GameObject::Create(std::string("PlayerUI" + id))->AddComponent<TextObject>();
	text->SetText("#lives: " + std::to_string(health) + "\n" + std::to_string(score), ResourceManager::GetInstance().LoadFont("Lingua.otf", 30));

	if (id == 0) {
		text->gameObject->transform->position = Vector3(5, 100, 0);
	}
	else if (id == 1) {
		text->gameObject->transform->position = Vector3(5, 180, 0);
	}

	PlayerUI* obs = text->gameObject->AddComponent<PlayerUI>();
	obs->Init(this);
	subject->RegisterObserver(obs);

	EventManager::GetInstance().AddListener(this, Event::Score_Increase);
	EventManager::GetInstance().AddListener(obs, Event::Score_Change);
	EventManager::GetInstance().AddListener(obs, Event::Player_Hurt);
}

void Player::Update()
{
	if (id == 0) {
		gameObject->transform->localRotation += 50 * Time::deltaTime;
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

void Player::IncreaseScore(int scoreInc)
{
	score += scoreInc;
	subject->Notify(Event::Score_Change, EventArgs({id, score}));
	EventManager::GetInstance().SendMessage(Event::Score_Change, EventArgs({ uint16_t(GetId()), score }));
}

void Player::TakeDamage(int dmg)
{
	if (!isDead) {
		health -= dmg;
		//subject->Notify(Event::Player_Hurt, EventArgs({id, health}));
		EventManager::GetInstance().SendMessage(Event::Player_Hurt, EventArgs({ uint16_t(GetId()), health }));
		if (health <= 0) {
			Die();
		}
	}
}

void Player::Die()
{
	subject->Notify(Event::Player_Die, EventArgs({id}));
}

void Player::HandleEvent(Event, EventArgs args)
{
	//...
	if (args[0] == id) IncreaseScore(args[1]);
}

void HurtCommand::ExecuteDown()
{
	player->TakeDamage(dmg);
}

void ScoreCommand::ExecuteDown()
{
	// temp, example: in other class to send depending on which player did something
	EventManager::GetInstance().SendMessage(Event::Score_Increase, EventArgs({uint16_t(player->GetId()), 100}));
	//player->IncreaseScore(score);
}
