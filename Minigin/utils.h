#pragma once
#include <vector>
#include <string>

namespace utils {
	std::vector<std::string> Split(std::string text, const std::string delimiter);
	template <typename T>
	T OffsetEnum(T e, T max, int offset) {
		e = static_cast<T>(static_cast<int>(e) + offset);
		while (e > max) e = e - max;
		while (e < 0) e = e + max;
		return e;
	}
	template<typename T>
	T OffsetEnum(T e, T max, T exclude, int offset)
	{
		// this is bad
		int ie = static_cast<int>(e);
		int imax = static_cast<int>(max);
		int iex = static_cast<int>(exclude);

		ie = ie + offset;
		while (ie > imax) ie -= imax;
		while (ie < 0) ie += imax;
		if (ie == iex && offset > 0) ie += 1;
		if (ie == iex && offset < 0) ie += imax;
		return static_cast<T>(ie);
	}

}

