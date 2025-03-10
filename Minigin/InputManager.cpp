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
			//std::cout << "down" << std::endl;
			for (auto& command : commands) {
				if (KeyDown(command->GetKey())) {
					command->ExecuteDown();
				}
			}
		}
		if (e.type == SDL_KEYUP) {
			//KeyUp(e.key.keysym.sym);
			holdKeys.erase(std::remove(holdKeys.begin(), holdKeys.end(), e.key.keysym.sym), holdKeys.end());
			//std::cout << "up" << std::endl;
			for (auto& command : commands) {
				if (KeyUp(command->GetKey())) {
					command->ExecuteUp();
				}
			}
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	for (auto& command : commands) {
		if (KeyPressed(command->GetKey())) {
			command->ExecutePressed();
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
void InputManager::RegisterCommand(Command* command) {
	std::unique_ptr<Command> com = std::unique_ptr<Command>(command);
	//command->Register(gameobject, key);
	commands.emplace_back(std::move(com));
}