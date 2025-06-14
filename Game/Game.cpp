//#pragma comment(lib, "Minigin.lib")

#include <windows.h>
//#include <SDL.h>
#include <filesystem>
#include <string>
#include <memory>
#include <map>
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <Minigin.h>
#include "GameManager.h"

void load()
{
	ServiceLocator::RegisterAudio(new SDLAudio());

	SceneManager::GetInstance().CreateScene("Demo");

	GameObject::Create("GameManager")->AddComponent<GameManager>()->InitMenu();
}
int main(int, char* []) {
	Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}