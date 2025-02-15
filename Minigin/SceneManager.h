#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"


class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	Scene& CreateScene(const std::string& name);

	void Update();
	void FixedUpdate();
	void Render();

	void ClearScenes();

	std::shared_ptr<Scene> curScene;
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
	std::vector<std::shared_ptr<Scene>> m_scenes;
};

