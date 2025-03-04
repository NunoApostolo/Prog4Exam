#pragma once
#include "SceneManager.h"
#include "GameObject.h"

//class GameObject;
class Scene
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	void Add(std::shared_ptr<GameObject> object);
	void Remove(std::shared_ptr<GameObject> object);
	void RemoveAll();

	void Update();
	void FixedUpdate();
	void Render() const;

	std::shared_ptr<GameObject> GetObjPtr(GameObject* ptr);

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private:
	explicit Scene(const std::string& name);

	std::string m_name;
	std::vector <std::shared_ptr<GameObject>> m_objects{};

	static unsigned int m_idCounter;
};

