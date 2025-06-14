#pragma once
#include "Singleton.h"
#include "BaseComponent.h"
#include "Player.h"
#include "MapBlock.h"
#include "MapCreator.h"
#include "Enemy.h"
#include "Subject.h"
#include "Command.h"

enum class GameMode {
	Solo,
	Duo,
	Versus
};

const std::vector<std::vector<char>> alphabet
{
	{'A','B','C','D','E','F','G','H','I','J'},
	{'K','L','M','N','O','P','Q','R','S','T'},
	{'U','V','W','X','Y','Z','\n','\0'}
};

class GameManager : public BaseComponent
{
public:
	static GameManager& GetInstance() {
		return *instance;
	}

	void Awake() override;
	void InitMenu();
	void Init();
	void InitSolo();
	void InitCoop();
	void InitVS();
	void ShowMenu();
	void ShowHighScore();
	void SpawnEnemies();
	void Update() override;

	void SetMode(int offset);
	GameMode GetMode() { return mode; }

	void MoveToLetter(int x, int y);
	void SelectLetter();
	void RemoveLetter();

	Vector3 GetPosInGrid(int x, int y);
	void GetGridPos(const Vector3& pos, int& x, int& y);
	bool IsPathInDirectionFree(const Vector3& pos, const Direction dir);
	Vector3 GetRandomPos();

	Vector3 GetPlayerPos(int id = 0);
	Vector3 GetPlayerPos(const Vector3& pos);
	Player* GetPlayer(int id);

	void AddColliders(std::vector<MapBlock*> cols);
	void ClearColliders();
	bool CheckColliders(const Vector3& pos, const float unitCol);
	bool CheckColliders(const Vector3& pos, const float unitCol, CollisionDirection& dir);
	Player* CheckPlayerColliders(const Vector3& pos, const float unitCol);
	Enemy* CheckEnemyColliders(const Vector3& pos, const float unitCol);
	void CheckDiamond(GameObject* obj);

	void NotifyEnemyDeath(Enemy* en);
	void NotifyPlayerDeath(Player* pl);

	void RegisterBullet(Bullet* bullet);
	void RemoveBullet(Bullet* bullet);

	bool IsInMenu() { return menuPrnt->IsActive(); }
	bool IsInScore() { return scorePrtn->IsActive(); }

	void ReadHighScores();
	void WriteHighScore();

private:
	GameObject* menuPrnt{};
	GameObject* scorePrtn{};
	TextureRenderer* letterSquare{};
	uint8_t letterIndex{0};
	std::string scoreName{"Player"};
	int endScore{ 0 };

	std::vector<std::pair<std::string, int>> scores{};

	Vector3 GetPosInGridInternal(int x, int y);
	Player* player{};
	Player* player2{};
	std::vector<Enemy*> enemies{};
	std::vector<Bullet*> bullets{};
	std::vector<MapBlock*> blocks{};
	MapCreator mapCreator{};
	float mapSize{};
	int lvl{ 1 };
	GameMode mode{ GameMode::Solo };

	std::vector<Vector3> freePosList{};
	float dieTimer{ 0 };

	const int GRID_SIZE{ 18 };
	const float TOP_OFFSET{ 100 };

	static GameManager* instance;

};

enum class MenuControl {
	up,
	down,
	left,
	right,
	select,
	cancel
};

class MenuCommand : public Command
{
public:
	MenuCommand(GameObject* gameobject, GamePad btn, MenuControl control);
	MenuCommand(GameObject* gameobject, SDL_Keycode keyCode, MenuControl control);

	void ExecuteDown() override;

private:
	MenuControl ctrl{};
};
class ScoreCommand : public Command
{
public:
	ScoreCommand(GameObject* gameobject, GamePad btn, MenuControl control);
	ScoreCommand(GameObject* gameobject, SDL_Keycode keyCode, MenuControl control);

	void ExecuteDown() override;

private:
	MenuControl ctrl{};
};