#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
//#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "TextureRenderer.h"
#include "GameObject.h"

void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>("Back");
	go->AddComponent<TextureRenderer>()->SetTexture("background.tga");
	scene.Add(go);

	go = std::make_shared<GameObject>("Logo");
	go->AddComponent<TextureRenderer>()->SetTexture("logo.tga");
	go->transform->SetPosition(216, 180, 0);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<GameObject>("Text");
	go->AddComponent<TextObject>()->SetText("Programming 4 Assignment", font);
	go->transform->SetPosition(80, 20, 0);
	scene.Add(go);

	go = std::make_shared<GameObject>("FPS");
	go->AddComponent<TextObject>()->SetText("0", font);
	go->transform->SetPosition(0, 100, 0);
	scene.Add(go);

}
int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}