#pragma once
#include "BaseComponent.h"
#include "TextureRenderer.h"
#include "Player.h"

class IdleState;
class WanderState;
class ChaseState;
class Enemy;

class BaseEnemyState {
public:
	static std::unique_ptr<IdleState> idle;
	static std::unique_ptr<WanderState> wandering;
	static std::unique_ptr<ChaseState> chasing;

	virtual void Update(Enemy* owner) = 0;
protected:
	//Enemy* owner;

};
class IdleState : public BaseEnemyState {
public:
	void Update(Enemy* owner) override;
};
class WanderState : public BaseEnemyState {
public:
	void Update(Enemy* owner) override;
};
class ChaseState : public BaseEnemyState {
public:
	void Update(Enemy* owner) override;
private:
	float shootTimer{ 2.f };
};

class Enemy : public BaseComponent
{
	friend class ChaseState;
public:
	void Start() override;
	virtual void Init() = 0;
	void Update() override;
	void UpdateMovePath();
	void SetDirection();

	void SetState(BaseEnemyState* state);
	void CreatePath(const Vector3& dest);
	void ClearPath();
	int GetPathCount();

	virtual void Shoot() = 0;
	void TakeDamage(int dmg, Player* player);

	bool CheckCollision(const Vector3& pos, const float unitCol);
protected:
	BaseEnemyState* curState{ nullptr };
	TextureRenderer* tankTex{ nullptr };
	float texSize{};

	int hp{ 3 };

	Direction dir{Direction::None};
	std::vector<Vector3> path{};

	float speed{100};
};

class TankEnemy : public Enemy {
public: 
	void Init() override;
	void Shoot() override;
};

class MeleeEnemy : public Enemy {
public:
	void Init() override;
	void Shoot() override {}

protected:
};