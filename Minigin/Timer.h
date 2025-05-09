#pragma once
#include "Singleton.h"
#include "TimeManager.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <future>
using namespace std::chrono_literals;

class Timer : public Singleton<Timer>
{
public:
	Timer();
	~Timer();

	void StartTimer(std::chrono::milliseconds t, std::function<void(void)> func);
	void StopTimer(); // ??
	void UpdateTimers();

private:
	std::mutex mutex{};
	std::vector <std::pair<std::jthread, std::function<void(void)>>> timers{};
	bool running{ false };

};

