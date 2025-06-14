#pragma once
#include "GameObject.h"

class MapCreator
{
public:
	MapCreator();
	void Init();
	void LoadMap(const std::string& path, const float& size);
	void CleanUp();

private:
	GameObject* mapObj{};
	TextureRenderer* dia{};

};

