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
	bool MouseDown(int btn);
	bool MousePressed(int btn);
	bool MouseUp(int btn);
	float GetScrollDelta();
	Vector2 GetMousePos();
	Vector2 GetWorldMousePos();

	void RegisterCommand(Command* command);
private:
	SDL_Keycode keyDown{};
	SDL_Keycode keyUp{};

	std::vector<std::unique_ptr<Command>> commands;
	std::vector<SDL_Keycode> holdKeys;
	Vector2 mousePos{};
	bool mouseDown{false};
	bool mouseHold{false};
	bool mouseUp{false};
	bool mouse2Down{ false };
	bool mouse2Hold{ false };
	bool mouse2Up{ false };
	float scrollDelta{ 0 };
	//std::vector<WORD> holdBtns;
	//std::vector<WORD> prevBtns;
	//SDL_Keycode downKey;
};


