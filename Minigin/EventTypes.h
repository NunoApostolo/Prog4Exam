#pragma once
#include <string>
#include <memory>
#include <vector>
#include <cassert>
//...?
enum class Event {
	Player_Hurt = 0,
	Score_Change = 1,
	Score_Increase = 2,
	Player_Die = 3
};

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