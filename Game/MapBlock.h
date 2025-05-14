#pragma once
#include "BaseComponent.h"
#include "Player.h"

enum class CollisionDirection {
	Right = 0,
	Up = 1,
	Left = 2, 
	Down = 3
};

class MapBlock : public BaseComponent
{
public:
	void SetCollider(float col);
	bool CheckCollision(const Vector3& pos, const float unitCol, CollisionDirection& dir);
private:
	float collider{};

};

