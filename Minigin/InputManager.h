#pragma once
#include "Singleton.h"
#include <vector>
#include "Command.h"
#include <memory>

class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();
	bool KeyDown(SDL_Keycode key);
	bool KeyPressed(SDL_Keycode key);
	bool KeyUp(SDL_Keycode key);
	bool MouseDown(bool left);
	bool MousePressed(bool left);
	bool MouseUp(bool left);

	void RegisterCommand(Command* command);
private:
	SDL_Event e;

	std::vector<std::unique_ptr<Command>> commands;
	std::vector<SDL_Keycode> holdKeys;
	//std::vector<WORD> holdBtns;
	//std::vector<WORD> prevBtns;
	//SDL_Keycode downKey;
};


