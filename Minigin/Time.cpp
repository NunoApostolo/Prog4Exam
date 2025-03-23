//#include "pch.h"
#include "TimeManager.h"

float Time::deltaTime{};
float Time::fixedDeltaTime{};
float Time::trueDeltaTime{};
float Time::timeScale{ 1 };
float Time::timer{0};
float Time::returnScale{1};

void Time::SetDeltaTime(float trueDT)
{
	deltaTime = trueDT * timeScale;
	Time::trueDeltaTime = trueDT;
}

void Time::SetFixedDeltaTime(float fixedDT)
{
	fixedDeltaTime = fixedDT * timeScale;
}

void Time::SetTimeScale(float scale)
{
	timeScale = scale;
}

float Time::GetTimeScale()
{
	return timeScale;
}

void Time::SetTimeScaleTimed(float scale, float rScale, float time)
{
	timer = time;
	returnScale = rScale;
	timeScale = scale;
}

void Time::Update(float trueDT)
{
	if (returnScale == timeScale) return;
	if (timer <= 0) {
		timer = 0;
		timeScale = returnScale;
	}
	if (timer > 0) timer -= trueDT;
}
