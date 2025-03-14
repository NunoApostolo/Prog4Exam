#pragma once
#include "SceneManager.h"
#include "GameObject.h"

//class GameObject;
class Scene final
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	void Add(std::unique_ptr<GameObject> object);
	void Remove(std::unique_ptr<GameObject> object);
	void RemoveAll();

	void Update();
	void FixedUpdate();
	void Render() const;
	void RenderUI();

	GameObject* GetObjPtr(GameObject* ptr);
	template <class Pred>
	GameObject* FindObj(Pred pred) {
		for (auto& obj : m_objects) {
			if (pred(obj.get())) return obj.get();
		}
	}
	template <class Pred>
	int FindObjCount(Pred pred) {
		int count{ 0 };
		for (auto& obj : m_objects) {
			if (pred(obj.get())) ++count;
		}
		return count;
	}

	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

private:
	explicit Scene(const std::string& name);

	std::string m_name;
	std::vector <std::unique_ptr<GameObject>> m_objects{};

	static unsigned int m_idCounter;
};

