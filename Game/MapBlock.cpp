#include "MapBlock.h"
#include "GameObject.h"

void MapBlock::SetCollider(float col)
{
	collider = col;
}

bool MapBlock::CheckCollision(const Vector3& pos, const float unitCol)
{
	Vector3 selfPos{ gameObject->transform->position };
	if (pos.x+ unitCol >= selfPos.x - collider && pos.x- unitCol <= selfPos.x + collider
		&& pos.y+ unitCol >= selfPos.y - collider && pos.y- unitCol <= selfPos.y + collider) {
		return true;
	}
	return false;
}
