// class for managing time, stores deltaTime as static, allows it to be used anywhere
#pragma once
class Time
{
public:
	Time() = delete;
	Time(const Time&) = delete;
	Time(Time&&) = delete;
	Time& operator =(const Time&) = delete;
	Time operator =(Time) = delete;

	static float deltaTime;
	static float fixedDeltaTime;
	static float trueDeltaTime; // unscaled

	static void SetDeltaTime(float trueDT);
	static void SetFixedDeltaTime(float fixedDT);
	static void SetTimeScale(float scale);
	static float GetTimeScale();
	static void SetTimeScaleTimed(float scale, float returnScale, float time);

	static void Update(float trueDeltaTime);

private:
	static float timeScale;
	static float timer;
	static float returnScale;

};

