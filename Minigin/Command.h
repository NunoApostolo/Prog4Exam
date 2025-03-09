#include "SDL.h"
#include <functional>
#include <windows.h>
#include <Xinput.h>

#pragma once
class Command final
{
public:
	Command() = default;
	Command(const Command&) = delete;
	Command(Command&&) = delete;

	void Register(SDL_Keycode key, std::function<void()> function, std::function<bool(SDL_Keycode)> keyFunction = NULL);
	void Register(WORD btn, std::function<void()> function, std::function<bool(SDL_Keycode)> keyFunction = NULL);

	void Execute();

	SDL_Keycode GetKey();
	WORD GetBtn();
	std::function<bool(SDL_Keycode)> keyFunc{};
private:
	SDL_Keycode key{};
	WORD btn{};
	std::function<void()> func{NULL};
};

