#include "MapCreator.h"
#include <iostream>
#include <fstream>
#include "Console.h"
#include "utils.h"
#include "MapBlock.h"
#include "GameManager.h"

const int NODE_COUNT{ 18 };
MapCreator::MapCreator()
{
}

void MapCreator::Init()
{
	mapObj = GameObject::Create("MapPrtn");
	mapObj->AddComponent<TextureRenderer>()->SetTexture("Missing.png", Vector2(0, 0), 0);

	dia = GameObject::Create("Diamond")->AddComponent<TextureRenderer>();
	dia->SetTexture("Diamond.png", Vector2(0.5f, 0.5f), 1);
	dia->gameObject->transform->localScale = Vector2(0.4f, 0.4f);
	dia->gameObject->transform->localPosition = Vector2(0,100/2);
	dia->gameObject->SetActive(false);

	mapObj->SetActive(false);
}

// 18x18
void MapCreator::LoadMap(const std::string& path, const float& size)
{
	dia->gameObject->SetActive(true);
	mapObj->SetActive(true);

	std:: string fullpath = "../Data/Maps/" + path;
	//const auto filename = std::filesystem::path(fullpath).filename().string();
	std::ifstream ifStream{ fullpath };

	if (!ifStream) {
		Console().GetInstance().Log("Map Load Failed.");
		return;
	}

	std::vector<Vector2> points{};
	for (int i{ 1 }; i < NODE_COUNT; ++i) {
		points.emplace_back(Vector2(size / NODE_COUNT * i, 0));
		points.emplace_back(Vector2(0, size / NODE_COUNT * i));
		points.emplace_back(Vector2(size / NODE_COUNT * i, size));
		points.emplace_back(Vector2(size, size / NODE_COUNT * i));
	}

	std::vector<std::string> str{};
	std::string text{};
	int y = 1;

	while (!ifStream.eof()) {
		std::getline(ifStream, text);
		str = utils::Split(text, ",");
		for (auto& s : str) {
			int x = std::stoi(s);
			points.emplace_back(Vector2((size / NODE_COUNT) * x, (size / NODE_COUNT) * y));
		}
		++y;
	}

	if (mapObj->childrenPtr.size() != 0) {
		for (auto& c : mapObj->childrenPtr) {
			GameObject::Delete(c);
		}
	}
	mapObj->transform->position = Vector2(-size / 2, -size / 2 + 100/2); // temp
	mapObj->GetComponent<TextureRenderer>()->SetTexture("Maps/" + utils::Split(path, ".")[0] + ".png");


	std::vector<MapBlock*> blocks{};
	for (auto& p : points) {
		MapBlock* block = GameObject::Create("Block")->AddComponent<MapBlock>();
		//block->SetCollider(size / (NODE_COUNT+4) / 2);
		block->gameObject->SetParent(mapObj);
		block->gameObject->transform->localPosition = p;
		block->SetCollider(size / (NODE_COUNT + 1) / 2);
		blocks.emplace_back(block);
	}

	GameManager::GetInstance().AddColliders(blocks);
}

void MapCreator::CleanUp()
{
	dia->gameObject->SetActive(false);
	mapObj->SetActive(false);
	for (auto& b : mapObj->childrenPtr) {
		GameObject::Delete(b);
	}

}
