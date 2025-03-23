#pragma once

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_ttf.lib")

#include <string>
#include <functional>
#include "InputManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#include "Minigin.h"
#include "TextObject.h"
#include "TextureRenderer.h"
#include "Player.h"
#include "EventManager.h"

class Minigin
{
public:
	explicit Minigin(const std::string& dataPath);
	~Minigin();
	void Run(const std::function<void()>& load);

	Minigin(const Minigin& other) = delete;
	Minigin(Minigin&& other) = delete;
	Minigin& operator=(const Minigin& other) = delete;
	Minigin& operator=(Minigin&& other) = delete;
};
