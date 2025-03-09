#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
#pragma comment(lib, "XInput.lib") 

bool InputManager::ProcessInput()
{
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			if (std::find(holdKeys.begin(), holdKeys.end(), e.key.keysym.sym) == holdKeys.end()) {
				holdKeys.emplace_back(e.key.keysym.sym);
			}
			//KeyDown(e.key.keysym.sym);
			std::cout << "down" << std::endl;
		}
		if (e.type == SDL_KEYUP) {
			//KeyUp(e.key.keysym.sym);
			holdKeys.erase(std::remove(holdKeys.begin(), holdKeys.end(), e.key.keysym.sym), holdKeys.end());
			std::cout << "up" << std::endl;
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	DWORD dwResult{ 0 };
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		dwResult = XInputGetState(i, &state);

		if (holdBtns.size() != XUSER_MAX_COUNT) {
			holdBtns.resize(XUSER_MAX_COUNT);
		}

		if (dwResult == ERROR_SUCCESS)
		{
			prevBtns[i] = holdBtns[i];
			holdBtns[i] = state.Gamepad.wButtons;
		}
		else
		{
			
		}
	}

	for (auto& command : commands) {
		if (command->GetKey() != 0) {
			if (command->keyFunc == NULL) {
				if (KeyPressed(command->GetKey())) command->Execute();
			}
			else if (command->keyFunc((command->GetKey()))) command->Execute();
		}
		else {
			if (command->keyFunc == NULL) {
				if (BtnPressed(command->GetBtn())) command->Execute();
			}
			else if (command->keyFunc((command->GetBtn()))) command->Execute();
		}
	}

	return true;
}

bool InputManager::KeyDown(SDL_Keycode key)
{
	if (e.key.keysym.sym == key && e.key.state == SDL_PRESSED) return true;
	return false;
}

bool InputManager::KeyPressed(SDL_Keycode key)
{
	for (auto& hkey : holdKeys) {
		if (hkey == key) return true;
	}
	return false;
}

bool InputManager::KeyUp(SDL_Keycode key)
{
	if (e.key.keysym.sym == key && e.key.state == SDL_RELEASED) return true;
	return false;
}

bool InputManager::BtnDown(WORD btn, int user)
{
	return (!(prevBtns[user] ^ btn) && (holdBtns[user] ^ btn));
}

bool InputManager::BtnPressed(WORD btn, int user)
{
	return holdBtns[user] ^ btn;
}

bool InputManager::BtnUp(WORD btn, int user)
{
	return (prevBtns[user] ^ btn && !(holdBtns[user] ^ btn));
}

bool InputManager::MouseDown(bool left)
{
	if (left) {

	}
	return false;
}

bool InputManager::MousePressed(bool left)
{
	if (left) {

	}
	return false;
}

bool InputManager::MouseUp(bool left)
{
	if (left) {

	}
	return false;
}

void InputManager::RegisterCommand(SDL_Keycode key, std::function<void()> function, std::function<bool(SDL_Keycode)> keyFunction)
{
	auto command = std::unique_ptr<Command>(new Command());
	command->Register(key, function, keyFunction);
	commands.emplace_back(std::move(command));
}