#pragma once


class Random
{
public:
	static void SetRandomSeed();
	//static int GetRandom(int min, int max);
	static size_t GetRandom(size_t min, size_t max);
	//static float GetRandom(float min, float max);

};

