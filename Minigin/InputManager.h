#pragma once
#include "Singleton.h"
#include <vector>
#include "Command.h"
#include <memory>

class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();
	bool KeyDown(SDL_KeyCode key);
	bool KeyPressed(SDL_KeyCode key);
	bool KeyUp(SDL_KeyCode key);
	bool MouseDown(bool left);
	bool MousePressed(bool left);
	bool MouseUp(bool left);

	void RegisterCommand(SDL_KeyCode key, std::function<void()> function, std::function<bool(SDL_KeyCode)> keyFunction = NULL);
;
private:
	std::vector<std::unique_ptr<Command>> commands;

};


