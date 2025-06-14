#pragma once
#include "MapBlock.h"
#include "GameObject.h"
#include "Console.h"

void MapBlock::SetCollider(float col)
{
	collider = col - 0;
	//TextureRenderer* tex = GameObject::Create("test")->AddComponent<TextureRenderer>();
	//tex->gameObject->transform->position = gameObject->transform->position + Vector3(collider,0,0);
	//tex->SetTexture("",Vector2(0.5f,0.5f),1000);
	//tex->gameObject->transform->scale = Vector2(0.1f, 0.1f);

	//tex = GameObject::Create("test")->AddComponent<TextureRenderer>();
	//tex->gameObject->transform->position = gameObject->transform->position + Vector3(-collider, 0, 0);
	//tex->SetTexture("", Vector2(0.5f, 0.5f), 1000);
	//tex->gameObject->transform->scale = Vector2(0.1f, 0.1f);

	//tex = GameObject::Create("test")->AddComponent<TextureRenderer>();
	//tex->gameObject->transform->position = gameObject->transform->position + Vector3(0, collider, 0);
	//tex->SetTexture("", Vector2(0.5f, 0.5f), 1000);
	//tex->gameObject->transform->scale = Vector2(0.1f, 0.1f);

	//tex = GameObject::Create("test")->AddComponent<TextureRenderer>();
	//tex->gameObject->transform->position = gameObject->transform->position + Vector3(0, -collider, 0);
	//tex->SetTexture("", Vector2(0.5f, 0.5f), 1000);
	//tex->gameObject->transform->scale = Vector2(0.1f, 0.1f);
}

bool MapBlock::CheckCollision(const Vector3& pos, const float unitCol, CollisionDirection& dir)
{
	Vector3 selfPos{ gameObject->transform->GetPosition()};
	if (pos.x + unitCol >= selfPos.x - collider && pos.x - unitCol <= selfPos.x + collider
		&& pos.y + unitCol >= selfPos.y - collider && pos.y - unitCol <= selfPos.y + collider) {

		float angle = Vector3().AngleDeg(pos, selfPos);
		if (angle <= 45 || angle >= 315) dir = CollisionDirection::Right;
		else if (angle <= 135) dir = CollisionDirection::Up;
		else if (angle <= 225) dir = CollisionDirection::Left;
		else if (angle <= 315) dir = CollisionDirection::Down;
		return true;
	}
	return false;
}
