#include "Bullet.h"
#include "GameObject.h"
#include <math.h>
#include "GameManager.h"
#include "Console.h"

void Bullet::Init(const Vector3& pos, const float rotation, const std::string& texStr)
{
	gameObject->transform->position = pos;
	dir = Vector2(std::cosf(rotation / 180 * static_cast<float>(M_PI)), std::sinf(rotation / 180 * static_cast<float>(M_PI)));
	TextureRenderer* tex = gameObject->AddComponent<TextureRenderer>();
	tex->SetTexture(texStr, Vector2(0.5f, 0.5f), 4);
	bulletSize = tex->GetSize().x/2;

	gameObject->transform->position += dir * 5;

}

void Bullet::Update()
{
	CollisionDirection colDir{};
	Vector3 nextpos = gameObject->transform->position + dir * (SPEED * Time::deltaTime);

	if (GameManager::GetInstance().CheckColliders(nextpos, bulletSize, colDir)) {
		if (colDir == CollisionDirection::Right || colDir == CollisionDirection::Left) {
			dir.x = dir.x * -1;
		}
		else {
			dir.y = dir.y * -1;
		}
		++bounceCount;
	}

	Vector3 prevPos = gameObject->transform->position;
	gameObject->transform->position += dir * (SPEED * Time::deltaTime);

	if (GameManager::GetInstance().CheckColliders(gameObject->transform->GetPosition(), bulletSize)) {
 		dir = dir * -1;
		gameObject->transform->position = prevPos + dir * (SPEED * Time::deltaTime);
	}

	if (bounceCount >= 3) GameObject::Delete(gameObject);
}
