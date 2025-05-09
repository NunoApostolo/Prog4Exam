#include "Bullet.h"
#include "GameObject.h"
#include <math.h>
#include "GameManager.h"

void Bullet::Init(const Vector3& pos, const float rotation, const std::string& tex)
{
	gameObject->transform->position = pos;
	dir = Vector2(std::cosf(rotation / 180 * static_cast<float>(M_PI)), std::sinf(rotation / 180 * static_cast<float>(M_PI)));
	gameObject->AddComponent<TextureRenderer>()->SetTexture(tex, Vector2(0.5f, 0.5f), 4);
}

void Bullet::Update()
{
	Vector3 nextpos = gameObject->transform->position + Vector2(dir.x,0) * (SPEED * Time::deltaTime);
	if (GameManager::GetInstance().CheckColliders(nextpos, 20)) {
		dir.x = dir.x * -1;
		++bounceCount;
	}

	nextpos = gameObject->transform->position + Vector2(0, dir.y) * (SPEED * Time::deltaTime);
	if (GameManager::GetInstance().CheckColliders(nextpos, 20)) {
		dir.y = dir.y * -1;
		++bounceCount;
	}

	gameObject->transform->position += dir * (SPEED * Time::deltaTime);

	if (bounceCount >= 3) GameObject::Delete(gameObject);
}
