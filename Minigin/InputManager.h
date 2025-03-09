#pragma once
#include "Singleton.h"
#include <vector>
#include "Command.h"
#include <memory>
#include <windows.h>
#include <Xinput.h>

class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();
	bool KeyDown(SDL_Keycode key);
	bool KeyPressed(SDL_Keycode key);
	bool KeyUp(SDL_Keycode key);
	bool BtnDown(WORD btn, int user = 0);
	bool BtnPressed(WORD btn, int user = 0);
	bool BtnUp(WORD btn, int user = 0);
	bool MouseDown(bool left);
	bool MousePressed(bool left);
	bool MouseUp(bool left);

	void RegisterCommand(SDL_Keycode key, std::function<void()> function, std::function<bool(SDL_Keycode)> keyFunction = NULL);
;
private:
	XINPUT_STATE state;
	SDL_Event e;

	std::vector<std::unique_ptr<Command>> commands;
	std::vector<SDL_Keycode> holdKeys;
	std::vector<WORD> holdBtns;
	std::vector<WORD> prevBtns;
	//SDL_Keycode downKey;
};


