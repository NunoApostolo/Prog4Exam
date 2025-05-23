#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

class Scene;
class SceneManager final : public Singleton<SceneManager>
{
public:
	~SceneManager() = default;

	Scene& CreateScene(const std::string& name);

	void Update();
	void FixedUpdate();
	void Render();
	void RenderUI();

	void ClearScenes();

	Scene* curScene{};
private:
	friend class Singleton<SceneManager>;
	SceneManager() = default;
};

