#pragma once
#include <string>
#include <memory>
#include <vector>
#include <cassert>
//...?
enum class Event {
	MouseMove = 0,
	MouseDown = 1,
	MouseUp = 2,

	End
};
//struct Event {
//	uint32_t e;
//};

struct EventArgs {
	static const uint16_t MAX_ARGS{ 5 };
	std::vector<uint16_t> args = std::vector<uint16_t>(MAX_ARGS, 0);

	EventArgs() {}
	EventArgs(const EventArgs& e) {
		args = e.args;
	}
	EventArgs(std::initializer_list<int> list) {
		assert(list.size() <= MAX_ARGS);
		for (size_t i{ 0 }; i < list.size(); ++i) {
			args[i] = (uint16_t) * (list.begin() + i);
		}
	}

	uint16_t operator [] (int idx) {
		assert(idx < MAX_ARGS);
		return args[idx];
	}
};