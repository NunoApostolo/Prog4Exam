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

	void SetEnemy(Enemy* enemy);
	virtual void Update() = 0;
protected:
	Enemy* owner;

};
class IdleState : public BaseEnemyState {
public:
	void Update() override;
};
class WanderState : public BaseEnemyState {
public:
	void Update() override;
};
class ChaseState : public BaseEnemyState {
public:
	void Update() override;
private:
	float shootTimer{ 2.f };
};

class Enemy : public BaseComponent
{
public:
	void Start() override;
	void Update() override;
	void UpdateMovePath();
	void SetDirection();

	void SetState(BaseEnemyState* state);
	void CreatePath(const Vector3& dest);
	void ClearPath();
	int GetPathCount();

	void Shoot();
	void TakeDamage(int dmg, Player* player);

	bool CheckCollision(const Vector3& pos, const float unitCol);
private:
	BaseEnemyState* curState{ nullptr };
	TextureRenderer* tankTex{ nullptr };
	float texSize{};

	int hp{ 3 };

	Direction dir{Direction::None};
	std::vector<Vector3> path{};

	const float SPEED{100};
};

