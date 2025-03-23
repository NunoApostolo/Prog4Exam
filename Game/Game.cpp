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
void load()
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");
	SceneManager::GetInstance().CreateScene("Demo");
	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);

	auto tank1 = GameObject::Create("Tank1");
	tank1->AddComponent<TextureRenderer>()->SetTexture("Tank.png");
	tank1->GetComponent<TextureRenderer>()->SetPivot(Vector2(0.5,0.5));
	//go->AddComponent <MoveObjTest>();
	tank1->AddComponent<Player>()->Init(0);
	tank1->transform->position = Vector3(0, 0, 0);
	//tank1->transform->localRotation = 45;
	//tank1->transform->scale = Vector2(2, 1);

	auto tank2 = GameObject::Create("Tank2");
	//go2->SetParent(go);
	tank2->AddComponent<TextureRenderer>()->SetTexture("Tank.png");
	//go2->AddComponent <MoveObjTest>();
	tank2->AddComponent<Player>()->Init(1);
	tank2->transform->position = Vector3(280, 200, 0);

	auto obj = GameObject::Create("ControlsUI");
	obj->AddComponent<TextObject>()->SetText(
		"WASD to move tank 1, C to take damage, Z to gain score\nD-Pad to move tank 2, X to take damage, A to gain score", font);
	obj->transform->position = Vector3(10, 10, 0);

}
int main(int, char* []) {
	Minigin engine("../Data/");
	engine.Run(load);
	return 0;
}