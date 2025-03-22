#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h" // temp
#include "Time.h"

#include <algorithm>

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name), m_objects{} {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	GameObject::CreateObjects(this);
	for(auto& object : m_objects)
	{
		object.get()->Update();
	}
	GameObject::DeleteObjects(this);
}

void Scene::FixedUpdate()
{

}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}
void Scene::RenderUI() {
	for (const auto& object : m_objects)
	{
		object->RenderUI();
	}
}

GameObject* Scene::GetObjPtr(GameObject* ptr)
{
	for (auto& obj : m_objects) {
		if (obj.get() == ptr) return obj.get();
	}
	return nullptr;
}