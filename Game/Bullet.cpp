#pragma once
#include "Bullet.h"
#include "GameObject.h"
#include <math.h>
#include "GameManager.h"
#include "Console.h"
#include "ServiceLocator.h"

void Bullet::Init(const Vector3& pos, const float rotation, const std::string& texStr)
{
	gameObject->transform->position = pos;
	dir = Vector2(std::cosf(rotation / 180 * static_cast<float>(M_PI)), std::sinf(rotation / 180 * static_cast<float>(M_PI)));
	TextureRenderer* tex = gameObject->AddComponent<TextureRenderer>();
	tex->SetTexture(texStr, Vector2(0.5f, 0.5f), 4);
	bulletSize = tex->GetSize().x/2;

	gameObject->transform->position += dir * 5;

	gameObject->transform->localRotation = rotation;

	GameManager::GetInstance().RegisterBullet(this);
	ServiceLocator::GetAudioService().PlayAudio("shoot.mp3", 0.5f);
}

void Bullet::Update()
{
	CollisionDirection colDir{};
	Vector3 nextpos = gameObject->transform->position + dir * (SPEED * Time::deltaTime);

	if (plOwner != nullptr) {
		if (GameManager::GetInstance().GetMode() == GameMode::Versus) {
			Player* pl{ GameManager::GetInstance().CheckPlayerColliders(nextpos, bulletSize) };
			if (pl != nullptr && pl != plOwner) {
				ServiceLocator::GetAudioService().PlayAudio("hit.mp3", 0.5f);
				GameManager::GetInstance().RemoveBullet(this);
				GameObject::Delete(gameObject);

				pl->TakeDamage(dmg);
			}
		}
		else {
			Enemy* enemy{ GameManager::GetInstance().CheckEnemyColliders(nextpos, bulletSize) };
			if (enemy != nullptr) {
				ServiceLocator::GetAudioService().PlayAudio("hit.mp3", 0.5f);
				GameManager::GetInstance().RemoveBullet(this);
				GameObject::Delete(gameObject);

				enemy->TakeDamage(dmg, plOwner);
				return;
			}
		}

	}
	else {
		Player* player{ GameManager::GetInstance().CheckPlayerColliders(nextpos, bulletSize) };
		if (player != nullptr) {
			//
			ServiceLocator::GetAudioService().PlayAudio("hit.mp3", 0.5f);
			GameManager::GetInstance().RemoveBullet(this);
			GameObject::Delete(gameObject);

			player->TakeDamage(dmg);
			return;
		}
	}

	if (GameManager::GetInstance().CheckColliders(nextpos, bulletSize, colDir)) {
		ServiceLocator::GetAudioService().PlayAudio("hit.mp3", 0.5f);
		if (colDir == CollisionDirection::Right || colDir == CollisionDirection::Left) {
			dir.x = dir.x * -1;
			gameObject->transform->localRotation += 180;
		}
		else {
			dir.y = dir.y * -1;
			gameObject->transform->localRotation += 180;
		}
		++bounceCount;
	}

	Vector3 prevPos = gameObject->transform->position;
	gameObject->transform->position += dir * (SPEED * Time::deltaTime);

	if (GameManager::GetInstance().CheckColliders(gameObject->transform->GetPosition(), bulletSize)) {
 		dir = dir * -1;
		gameObject->transform->position = prevPos + dir * (SPEED * Time::deltaTime);
	}

	if (bounceCount >= 5)
	{
		GameObject::Delete(gameObject);
		GameManager::GetInstance().RemoveBullet(this);
	}
}

void Bullet::SetOwner(Player* player)
{
	plOwner = player;
}

void Bullet::SetOwner(Enemy* enemy)
{
	enOwner = enemy;
}
