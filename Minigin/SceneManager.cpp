#pragma once
#include "SceneManager.h"
#include "Scene.h"

std::vector<std::unique_ptr<Scene>> m_scenes;

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
	auto scene = std::unique_ptr<Scene>(new Scene(name));
	curScene = scene.get();
	m_scenes.emplace_back(std::move(scene));
	return *curScene;
}