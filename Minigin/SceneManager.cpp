#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Update()
{
	for(auto& scene : m_scenes)
	{
		scene.get()->Update();
	}
}

void SceneManager::FixedUpdate()
{
	for (auto& scene : m_scenes)
	{
		scene->FixedUpdate();
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

void SceneManager::ClearScenes()
{
	for (const auto& scene : m_scenes) {
		scene->RemoveAll();
	}
}

Scene& SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	curScene = scene;
	m_scenes.push_back(scene);
	return *scene;
}
