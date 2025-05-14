#pragma once
#include "Singleton.h"
#include "BaseComponent.h"
#include "Player.h"
#include "MapBlock.h"
#include "MapCreator.h"
#include "Enemy.h"

class GameManager : public BaseComponent
{
public:
	static GameManager& GetInstance() {
		return *instance;
	}

	void Init();
	void Update() override;

	Vector3 GetPosInGrid(int x, int y);
	void GetGridPos(const Vector3& pos, int& x, int& y);
	bool IsPathInDirectionFree(const Vector3& pos, const Direction dir);
	Vector3 GetRandomPos();

	Vector3 GetPlayerPos();

	void AddColliders(std::vector<MapBlock*> cols);
	void ClearColliders();
	bool CheckColliders(const Vector3& pos, const float unitCol);
	bool CheckColliders(const Vector3& pos, const float unitCol, CollisionDirection& dir);

private:
	Vector3 GetPosInGridInternal(int x, int y);
	Player* player{};
	std::vector<MapBlock*> blocks{};
	MapCreator mapCreator{};
	float mapSize{};

	std::vector<Vector3> freePosList{};

	const int GRID_SIZE{ 18 };
	const float TOP_OFFSET{ 100 };

	static GameManager* instance;

};

