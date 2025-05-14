#pragma once
#include "BaseComponent.h"

class Bullet : public BaseComponent
{
public:

	void Init(const Vector3& pos, const float rotation, const std::string& texStr);
	void Update() override;

private:
	Vector2 dir{};
	const int SPEED{ 200 };
	int bounceCount{ 0 };
	float bulletSize{ 20 };

};

