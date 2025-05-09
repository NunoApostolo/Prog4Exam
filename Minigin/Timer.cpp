#include "Timer.h"

Timer::Timer()
{
}

Timer::~Timer()
{
	if (running) {
		StopTimer();
	}
}

void Timer::StartTimer(std::chrono::milliseconds t, std::function<void(void)> func)
{
	running = true;
	const std::chrono::milliseconds interval{100ms};
	//std::lock_guard<std::mutex> guard(mutex);
	//std::thread timer = std::thread([this, t, interval, func] {
	//	std::chrono::milliseconds total{ 0ms };
	//	while (running) {
	//		std::this_thread::sleep_for(interval);
	//		total += interval;
	//		if (total >= t) {
	//			StopTimer();
	//			func();
	//			running = false;
	//		}
	//	}
	//	});
	timers.emplace_back(std::pair<std::jthread, std::function<void(void)>>(std::jthread([this, t, interval] {
		std::chrono::milliseconds total{ 0ms };
		while (running) {
			std::this_thread::sleep_for(interval);
			total += interval;
			if (total >= t) {
				//StopTimer();
				//func();
				running = false;
			}
		}
		}), func));
	//thread.join();
}

void Timer::StopTimer()
{
	running = false;
	//timers.detach();
}

void Timer::UpdateTimers()
{
	int index{ 0 };
	for (auto& timer : timers) {
		if (timer.first.joinable()) {
			timer.second();
			//timer.first.detach();
			timers.erase(timers.begin() + index);
			return;
		}
		++index;
	}
	//timers.erase(std::find_if(timers.begin(), timers.end(), [](std::pair<std::thread, std::function<void(void)>>& pair) {
	//	if (pair.first.joinable()) {
	//		pair.second();
	//		return true;
	//	}
	//	return false; 
	//	}));
}
