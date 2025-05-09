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
}

// 18x18
void MapCreator::LoadMap(const std::string& path, const float& size)
{
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
		block->SetCollider(NODE_COUNT / size / 2);
		block->gameObject->SetParent(mapObj);
		block->gameObject->transform->localPosition = p;
		blocks.emplace_back(block);
	}

	GameManager::GetInstance().AddColliders(blocks);
}
