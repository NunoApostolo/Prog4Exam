#include "utils.h"

std::vector<std::string> utils::Split(std::string text, const std::string delimiter) {
	size_t tPos{ 0 };
	std::vector<std::string> args{};
	while ((tPos = text.find(delimiter)) != std::string::npos) { // gets array of anim arguments from line
		args.push_back(text.substr(0, tPos));
		text.erase(0, tPos + delimiter.length());
	}
	args.push_back(text.substr(0, text.length()));
	return args;
}