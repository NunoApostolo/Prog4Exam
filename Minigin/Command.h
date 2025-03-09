#include "SDL.h"
#include <functional>

#pragma once
class Command
{
public:

	void Register(SDL_KeyCode key, std::function<void()> function, std::function<bool(SDL_KeyCode)> keyFunction = NULL);

	void Execute();

	SDL_KeyCode GetKey();
	std::function<bool(SDL_KeyCode)> keyFunc;
private:
	SDL_KeyCode key;
	std::function<void()> func;
};

