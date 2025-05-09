#pragma once
#include "BaseComponent.h"
#include "Player.h"

class MapBlock : public BaseComponent
{
public:
	void SetCollider(float col);
	bool CheckCollision(const Vector3& pos, const float unitCol);
private:
	float collider{};

};

