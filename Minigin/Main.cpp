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
#include "FpsCounter.h"
#include "MoveObjTest.h"

void load()
{
	//auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	SceneManager::GetInstance().CreateScene("Demo");

	auto go = GameObject::CreateObject("Back");
	go->AddComponent<TextureRenderer>()->SetTexture("background.tga");

	go = GameObject::CreateObject("Back");
	go->AddComponent<TextureRenderer>()->SetTexture("logo.tga");
	go->transform->position = glm::vec3(216, 180, 0);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = GameObject::CreateObject("Back");
	go->AddComponent<TextObject>()->SetText("Programming 4 Assignment", font);
	go->transform->position = glm::vec3(80, 20, 0);

	go = GameObject::CreateObject("Back");
	go->AddComponent<TextObject>()->SetText("0", font);
	go->AddComponent<FpsCounter>();
	go->transform->position = glm::vec3(0, 100, 0);

	go = GameObject::CreateObject("Tank1");
	go->AddComponent<TextureRenderer>()->SetTexture("Tank.png");
	go->AddComponent <MoveObjTest>();
	go->transform->position = glm::vec3(300, 200, 0);

	auto go2 = GameObject::CreateObject("Tank2");
	go2->SetParent(go.get());
	go2->AddComponent<TextureRenderer>()->SetTexture("Tank.png");
	go2->AddComponent <MoveObjTest>();
	go2->transform->position = glm::vec3(0,0,0);
}
int main(int, char*[]) {
	Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}