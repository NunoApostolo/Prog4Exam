#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>
#include "Minigin.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Time.h"
#include "Minigin.h"
#include "TextObject.h"
#include "TextureRenderer.h"
#include "EventManager.h"
#include <iostream>

using namespace std::chrono;
SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		960,
		700,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);
}

Minigin::~Minigin()
{
	ResourceManager::GetInstance().UnloadUnusedResources();
	SceneManager::GetInstance().ClearScenes();
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& event = EventManager::GetInstance();

	nanoseconds lag{};
	nanoseconds preTime = duration_cast<nanoseconds>(high_resolution_clock().now().time_since_epoch());
	
	const int frameRate{ 60 };

	// todo: this update loop could use some work.
	bool doContinue = true;
	while (doContinue)
	{
		nanoseconds postTime = duration_cast<nanoseconds>(high_resolution_clock().now().time_since_epoch());
		Time::SetDeltaTime(duration<float>(postTime - preTime).count());
		preTime = postTime;

		event.Update();
		doContinue = input.ProcessInput();

		lag += duration_cast<nanoseconds>(postTime - preTime);
		if (lag >= 1000ns) {
			lag -= 1000ns;
			sceneManager.FixedUpdate();
		}

		sceneManager.Update();
		renderer.Render();

		postTime = duration_cast<nanoseconds>(high_resolution_clock().now().time_since_epoch());

		auto wait = postTime + milliseconds(1000/frameRate) - high_resolution_clock::now().time_since_epoch();

		//std::cout << wait << std::endl;

		if (wait > 0ms) std::this_thread::sleep_for(wait);
	}
}
