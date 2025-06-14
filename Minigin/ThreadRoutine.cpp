#pragma once
#include "ThreadRoutine.h"

std::vector<std::thread> ThreadRoutine::routines{};
std::mutex ThreadRoutine::mutex{};

void ThreadRoutine::StartRoutine(std::function<void()> func)
{
	routines.emplace_back(std::thread(InternRoutine,func));
	//for (auto& routine : routines) {
	//	routine.join();
	//}
	//routines.clear();
}


void ThreadRoutine::InternRoutine(std::function<void()> func)
{
	std::lock_guard<std::mutex> guard(mutex);
	func();
	//routines.erase(std::find(routines.begin(), routines.end(), func));
	routines.clear();
}
