#include "MoveObjTest.h"
#include "GameObject.h"
#include <math.h>

void MoveObjTest::Start()
{
	startPos = gameObject->transform->localPosition;
}

void MoveObjTest::Update()
{
	angle += Time::deltaTime;
	if (gameObject->parentPtr == nullptr) {
		gameObject->transform->localPosition.x = startPos.x + (50 * cos(angle));
		gameObject->transform->localPosition.y = startPos.y + (50 * sin(angle));
	}
	else {
		gameObject->transform->localPosition.x = (50 * sin(angle));
		gameObject->transform->localPosition.y = (50 * cos(angle));
	}

}

void MoveObjTest::Render() const
{

}
