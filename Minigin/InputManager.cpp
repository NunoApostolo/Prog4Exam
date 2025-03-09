#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"

SDL_Event e;
bool InputManager::ProcessInput()
{
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		
		for (auto& command : commands) {
			if (command->keyFunc == NULL) {
				if (KeyDown(command->GetKey())) command->Execute();
			}
			else if(command->keyFunc((command->GetKey()))) command->Execute();

		}

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}

bool InputManager::KeyDown(SDL_KeyCode key)
{
	if (e.key.keysym.sym == key && e.key.state == SDL_PRESSED) return true;
	return false;
}

bool InputManager::KeyPressed(SDL_KeyCode key)
{
	if (e.key.keysym.sym == key && e.key.state == SDL_PRESSED) return true;
	return false;
}

bool InputManager::KeyUp(SDL_KeyCode key)
{
	if (e.key.keysym.sym == key && e.key.state == SDL_RELEASED) return true;
	return false;
}

bool InputManager::MouseDown(bool left)
{
	return false;
}

bool InputManager::MousePressed(bool left)
{
	return false;
}

bool InputManager::MouseUp(bool left)
{
	return false;
}

void InputManager::RegisterCommand(SDL_KeyCode key, std::function<void()> function, std::function<bool(SDL_KeyCode)> keyFunction)
{
	Command* command = new Command();
	command->Register(key, function, keyFunction);
	commands.emplace_back(std::make_unique<Command>(command));
}
