#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h" // temp
#include "Time.h"
#include "TextureRenderer.h"

#include <algorithm>

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name), m_objects{} {}

void Scene::OrderObjects()
{
	std::sort(m_objects.begin(), m_objects.end(), [&](const std::unique_ptr<GameObject>& obj1, const std::unique_ptr<GameObject>& obj2) {
		int ord1{ obj1->GetOrder() };
		int ord2{ obj2->GetOrder() };

		return ord1 < ord2;
		});

	orderFlag = false;
}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(GameObject* object)
{
	m_objects.erase(std::find_if(m_objects.begin(), m_objects.end(), [object](std::unique_ptr<GameObject>& uptr) {return uptr.get() == object; }));
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update()
{
	GameObject::CreateObjects(this);
	if (orderFlag) OrderObjects();
	for (auto& object : m_objects)
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

void Scene::SetOrderFlag()
{
	orderFlag = true;
}

std::vector<GameObject*> Scene::GetAllObjs()
{
	std::vector<GameObject*> objs{};
	for (auto& obj : m_objects) {
		objs.emplace_back(obj.get());
	}
	for (auto& obj : GameObject::objToCreate) {
		objs.emplace_back(obj.get());
	}
	return objs;
}

GameObject* Scene::GetObjPtr(GameObject* ptr)
{
	for (auto& obj : m_objects) {
		if (obj.get() == ptr) return obj.get();
	}
	return nullptr;
}