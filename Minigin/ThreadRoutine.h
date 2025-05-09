#pragma once
#include <thread>
#include <mutex>
#include <functional>
#include <chrono>

using namespace std::chrono_literals;

class ThreadRoutine
{
public:
	//template<class T>
	static void StartRoutine(std::function<void(void)> func);
private:
	static std::vector<std::thread> routines;
	static std::mutex mutex;
	static void InternRoutine(std::function<void()> func);
};

