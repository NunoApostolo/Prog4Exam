#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Update()
{
	curScene->Update();
}

void SceneManager::FixedUpdate()
{
	curScene->FixedUpdate();
}

void SceneManager::Render()
{
	curScene->Render();
}

void SceneManager::RenderUI()
{
	curScene->RenderUI();
}

void SceneManager::ClearScenes()
{
	for (const auto& scene : m_scenes) {
		scene->RemoveAll();
	}
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::make_unique<Scene>(name);
	curScene = scene.get();
	m_scenes.emplace_back(scene);
	return *scene;
}
