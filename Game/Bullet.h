#pragma once
#include "BaseComponent.h"
class Player;
class Enemy;

class Bullet : public BaseComponent
{
public:

	void Init(const Vector3& pos, const float rotation, const std::string& texStr);
	void Update() override;

	void SetOwner(Player* player);
	void SetOwner(Enemy* enemy);

private:
	Vector2 dir{};
	const int SPEED{ 200 };
	int bounceCount{ 0 };
	float bulletSize{ 20 };
	int dmg{1};

	Player* plOwner{ nullptr };
	Enemy* enOwner{ nullptr };
};

