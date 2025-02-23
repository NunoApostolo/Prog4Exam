#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h" // temp
#include "Time.h"

#include <algorithm>

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name), m_objects{} {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	for(auto& object : m_objects)
	{
		object.get()->Update();
	}
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

std::shared_ptr<GameObject> Scene::GetObjPtr(GameObject* ptr)
{
	for (auto& obj : m_objects) {
		if (obj.get() == ptr) return obj;
	}
	return nullptr;
}

