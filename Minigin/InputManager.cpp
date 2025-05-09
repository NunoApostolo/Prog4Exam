#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>
//#pragma comment(lib, "XInput.lib") 
#include "XInput.h"
#include "EventManager.h"
#include "Camera.h"

bool InputManager::ProcessInput()
{
	SDL_Event e;
	keyDown = 0;
	keyUp = 0;
	mouseDown = false;
	mouseUp = false;
	scrollDelta = 0;
	while (SDL_PollEvent(&e)) {
		
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_MOUSEMOTION) {
			mousePos.x = static_cast<float>(e.motion.x);
			mousePos.y = static_cast<float>(e.motion.y);
			EventManager::GetInstance().SendMessage(Event::MouseMove, EventArgs({ -1, (int)e.motion.x, (int)e.motion.y }));
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				mouseDown = true;
				mouseHold = true;
				EventManager::GetInstance().SendMessage(Event::MouseDown, EventArgs({ 0, (int)e.motion.x, (int)e.motion.y }));
			}
			if (e.button.button == SDL_BUTTON_RIGHT) {
				mouse2Down = true;
				mouse2Hold = true;
				EventManager::GetInstance().SendMessage(Event::MouseDown, EventArgs({1, (int)e.motion.x, (int)e.motion.y }));
			}

		}
		if (e.type == SDL_MOUSEBUTTONUP) {
			if (e.button.button == SDL_BUTTON_LEFT) {
				mouseHold = false;
				mouseUp = true;
				EventManager::GetInstance().SendMessage(Event::MouseUp, EventArgs({ 0, (int)e.motion.x, (int)e.motion.y }));
			}
			if (e.button.button == SDL_BUTTON_RIGHT) {
				mouse2Hold = false;
				mouse2Up = true;
				EventManager::GetInstance().SendMessage(Event::MouseUp, EventArgs({ 1, (int)e.motion.x, (int)e.motion.y }));
			}
		}
		if (e.type == SDL_MOUSEWHEEL) {
			scrollDelta = e.wheel.preciseY;
			//event
		}
		if (e.type == SDL_KEYDOWN) {
			if (std::find(holdKeys.begin(), holdKeys.end(), e.key.keysym.sym) == holdKeys.end()) {
				holdKeys.emplace_back(e.key.keysym.sym);
				keyDown = e.key.keysym.sym;
				for (auto& command : commands) {
					if (KeyDown(command->GetKey())) {
						command->ExecuteDown();
					}
				}
			}
			//KeyDown(e.key.keysym.sym);
			//std::cout << "down" << std::endl;

		}
		if (e.type == SDL_KEYUP) {
			//KeyUp(e.key.keysym.sym);
			holdKeys.erase(std::remove(holdKeys.begin(), holdKeys.end(), e.key.keysym.sym), holdKeys.end());
			keyUp = e.key.keysym.sym;
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

	XInput::GetInstance().ProcessInput();

	for (auto& command : commands) {
		if (command->isGamePad) XInput::GetInstance().ProcessCommand(command.get());
		if (KeyPressed(command->GetKey())) {
			command->ExecutePressed();
		}
	}

	return true;
}

bool InputManager::KeyDown(SDL_Keycode key)
{
	if (keyDown == key /*&& sdlEvent.key.state == SDL_PRESSED*/) return true;
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
	if (keyUp == key /*&& sdlEvent.key.state == SDL_RELEASED*/) return true;
	return false;
}

bool InputManager::MouseDown(int btn)
{
	if (btn == 0) {
		return mouseDown;
	}
	return false;
}

bool InputManager::MousePressed(int btn)
{
	if (btn == 0) {
		return mouseHold;
	}
	return false;
}

bool InputManager::MouseUp(int btn)
{
	if (btn == 0) {
		return mouseUp;
	}
	return false;
}
float InputManager::GetScrollDelta()
{
	return scrollDelta;
}
Vector2 InputManager::GetMousePos() {
	return mousePos;
}
Vector2 InputManager::GetWorldMousePos() {
	Vector2 halfPos{ -Camera::GetMainCamera().GetViewPort().width / 2.f , -Camera::GetMainCamera().GetViewPort().height / 2.f };
	return (mousePos + Camera::GetMainCamera().gameObject->transform->GetPosition()) + halfPos;
}


void InputManager::RegisterCommand(Command* command) {
	std::unique_ptr<Command> com = std::unique_ptr<Command>(command);
	//command->Register(gameobject, key);
	commands.emplace_back(std::move(com));
}