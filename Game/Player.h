#pragma once
#include "BaseComponent.h"
#include "InputManager.h"
#include "Subject.h"
#include "Command.h"
#include "EventManager.h"

class Player : public BaseComponent, public IEventHandler
{
public:
	Player(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	void Awake() override;
	void Start() override;
	void Update() override;

	void Init(int id, int health = 1, int lives = 3);
	void IncreaseScore(int scoreInc);
	void TakeDamage(int dmg);
	void Die();

	int GetHealth() { return health;}
	int GetLives() { return lives;}
	int GetScore() { return score; }
	int GetId() { return id; }

	void HandleEvent(Event e, EventArgs args) override;
private:
	int id{ 0 };
	int health{ 3 };
	float speed{ 100 };
	int score{ 0 };
	int lives{ 3 };
	bool isDead{ false };
	std::unique_ptr<Subject> subject;

};

class ScoreCommand : public Command {
public:
	ScoreCommand(GameObject* gameobject, GamePad btn, int score) :
		Command(gameobject, btn), score{ score } 
	{
		player = gameobject->GetComponent<Player>();
	}
	ScoreCommand(GameObject* gameobject, SDL_Keycode keyCode, int score):
		Command(gameobject, keyCode), score{ score }
	{
		player = gameobject->GetComponent<Player>();
	}

	void ExecuteDown() override;

private:
	int score;
	Player* player{};
};
class HurtCommand : public Command {
public:
	HurtCommand(GameObject* gameobject, GamePad btn, int dmg) :
		Command(gameobject, btn), dmg{ dmg } 
	{
		player = gameobject->GetComponent<Player>();
	}
	HurtCommand(GameObject* gameobject, SDL_Keycode keyCode, int dmg) :
		Command(gameobject, keyCode), dmg{ dmg }
	{
		player = gameobject->GetComponent<Player>();
	}

	void ExecuteDown() override;

private:
	int dmg;
	Player* player{};
};