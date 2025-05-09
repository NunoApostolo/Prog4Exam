#pragma once
#include "Singleton.h"
#include "BaseComponent.h"
#include "Player.h"
#include "MapBlock.h"
#include "MapCreator.h"

class GameManager : public BaseComponent, public Singleton<GameManager>
{
public:
	void Init();
	void Update() override;

	void AddColliders(std::vector<MapBlock*> cols);
	void ClearColliders();
	bool CheckColliders(const Vector3& pos, const float unitCol);

private:
	Player* player{};
	std::vector<MapBlock*> blocks{};
	MapCreator mapCreator{};

};

