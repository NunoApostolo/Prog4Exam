#pragma once
#include "BaseComponent.h"
#include "InputManager.h"
#include "Subject.h"
#include "Command.h"
#include "EventManager.h"
#include "Bullet.h"

enum class Direction {
	None = 0,
	Right = 1,
	Up = 2,
	Left = 3,
	Down = 4
};

class Player : public BaseComponent, public IEventHandler
{
	friend class PlayerMoveCommand;
public:
	void Awake() override;
	void Start() override;
	void Update() override;

	void Init(int id, int health = 1, int lives = 3);
	~Player();
	void Spawn();

	void TurnBarrelLeft();
	void TurnBarrelRight();
	void SetBarrelDir(const Vector2& barrelDir);
	void SetDirection(Direction direction);
	bool CheckCollision(const Vector3& pos, const float unitCol);

	void Shoot();

	void IncreaseScore(int scoreInc);
	void TakeDamage(int dmg);
	void Die();

	int GetHealth() { return health;}
	bool IsDead() { return isDead; }
	int GetLives() { return lives;}
	int GetScore() { return score; }
	int GetId() { return id; }

	void HandleEvent(Event e, EventArgs args) override;
protected:
	Direction dir{Direction::Right};

private:
	const int BARREL_ROT_SPEED{ 100 };

	int id{ 0 };
	int health{ 1 };
	float speed{ 120 };
	int score{ 0 };
	int lives{ 3 };
	bool isDead{ false };
	float texSize{};
	std::unique_ptr<Subject> subject;
	TextureRenderer* tex{}, * barrelTex{};
	GameObject* ui{};

};
class PlayerMoveCommand : public Command
{
public:
	PlayerMoveCommand(GameObject* gameobject, GamePad btn, Vector3 moveVector, Direction direction);
	PlayerMoveCommand(GameObject* gameobject, SDL_Keycode keyCode, Vector3 moveVector, Direction direction);

	void ExecuteDown() override;
	void ExecutePressed() override;
	void ExecuteUp() override;

private:
	Vector3 moveVec{};
	Player* player{};
	Direction dir{};
};

class ShootCommand : public Command {
public:
	ShootCommand(GameObject* gameobject, GamePad btn, int dmg) :
		Command(gameobject, btn), dmg{ dmg } 
	{
		player = gameobject->GetComponent<Player>();
	}
	ShootCommand(GameObject* gameobject, SDL_Keycode keyCode, int dmg) :
		Command(gameobject, keyCode), dmg{ dmg }
	{
		player = gameobject->GetComponent<Player>();
	}

	void ExecuteDown() override;

private:
	int dmg;
	Player* player{};
};
class BarrelCommand : public Command {
public:
	BarrelCommand(GameObject* gameobject, GamePad btn, bool isLeft) :
		Command(gameobject, btn), isLeft{ isLeft }
	{
		player = gameobject->GetComponent<Player>();
	}
	BarrelCommand(GameObject* gameobject, SDL_Keycode keyCode, bool isLeft) :
		Command(gameobject, keyCode), isLeft{ isLeft }
	{
		player = gameobject->GetComponent<Player>();
	}

	void ExecutePressed() override;
	void MoveRThumb(const Vector2& thumb) override;

private:
	bool isLeft;
	Player* player{};
};