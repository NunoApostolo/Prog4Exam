#include "GameManager.h"
#include "InputManager.h"
#include "ServiceLocator.h"
#include "Camera.h"
#include "Console.h"
#include "Random.h"
#include "TextObject.h"
#include <fstream>
#include "utils.h"

GameManager* GameManager::instance{};

void GameManager::Awake()
{
	instance = this;
	Random::SetRandomSeed();
	mapSize = static_cast<float>(Camera::GetMainCamera().GetViewPort().width);
}

void GameManager::InitMenu()
{
	menuPrnt = GameObject::Create("menu");
	GameObject::Create("menu modes prnt")->SetParent(menuPrnt);

	mapCreator.Init();
	
	TextureRenderer* tex = GameObject::Create("Title")->AddComponent<TextureRenderer>();
	tex->SetTexture("Title.png", Vector2(0.5f, 0.5f), 10);
	tex->gameObject->SetParent(menuPrnt);
	tex->gameObject->transform->localPosition = Vector2(0, -250);

	TextObject* text = GameObject::Create("single text")->AddComponent<TextObject>();
	text->gameObject->SetParent(menuPrnt->childrenPtr[0]);
	text->SetText("Single Player", ResourceManager::GetInstance().LoadFont("", 40),1, TextAllingmentType::Center);
	tex = GameObject::Create("single arrow")->AddComponent<TextureRenderer>();
	tex->gameObject->SetParent(text->gameObject);
	tex->SetTexture("Arrow.png",Vector2(1,0.5f),1);
	tex->gameObject->transform->localPosition = Vector2(-text->GetSize().x / 2 - 20, 0);
	tex->SetSize(Vector2(text->GetSize().y, text->GetSize().y));

	text = GameObject::Create("co-op text")->AddComponent<TextObject>();
	text->gameObject->SetParent(menuPrnt->childrenPtr[0]);
	text->gameObject->transform->localPosition = Vector2(0, 50);
	text->SetText("Co-op", ResourceManager::GetInstance().LoadFont("", 40), 1, TextAllingmentType::Center);
	tex = GameObject::Create("co-op arrow")->AddComponent<TextureRenderer>();
	tex->gameObject->SetParent(text->gameObject);
	tex->SetTexture("Arrow.png", Vector2(1, 0.5f), 1);
	tex->gameObject->transform->localPosition = Vector2(-text->GetSize().x / 2 - 20, 0);
	tex->SetSize(Vector2(text->GetSize().y, text->GetSize().y));
	tex->gameObject->SetActive(false);

	text = GameObject::Create("vs text")->AddComponent<TextObject>();
	text->gameObject->SetParent(menuPrnt->childrenPtr[0]);
	text->gameObject->transform->localPosition = Vector2(0, 100);
	text->SetText("Versus", ResourceManager::GetInstance().LoadFont("", 40), 1, TextAllingmentType::Center);
	tex = GameObject::Create("vs arrow")->AddComponent<TextureRenderer>();
	tex->gameObject->SetParent(text->gameObject);
	tex->SetTexture("Arrow.png", Vector2(1, 0.5f), 1);
	tex->gameObject->transform->localPosition = Vector2(-text->GetSize().x / 2 - 20, 0);
	tex->SetSize(Vector2(text->GetSize().y, text->GetSize().y));
	tex->gameObject->SetActive(false);

	scorePrtn = GameObject::Create("score prnt");
	GameObject* lettersPrtn = GameObject::Create("letters prtn");
	lettersPrtn->SetParent(scorePrtn);
	lettersPrtn->transform->position = Vector2(-mapSize / 2 + mapSize / alphabet[0].size()/2, 200);

	text = GameObject::Create("Name")->AddComponent<TextObject>();
	text->gameObject->SetParent(scorePrtn);
	text->SetText("", ResourceManager::GetInstance().LoadFont("", 30), 5, TextAllingmentType::Center);
	text->gameObject->transform->localPosition = Vector2(0, 100);

	text = GameObject::Create("Score")->AddComponent<TextObject>();
	text->gameObject->SetParent(scorePrtn);
	text->SetText("0000", ResourceManager::GetInstance().LoadFont("", 30), 5, TextAllingmentType::Center);
	text->gameObject->transform->localPosition = Vector2(0, 50);

	GameObject* scoreListPrtn = GameObject::Create("scorelist");
	scoreListPrtn->SetParent(scorePrtn);
	scoreListPrtn->transform->localPosition = Vector2(-200, -350);

	Vector2 size{};
	for (int y{ 0 }; y < alphabet.size(); ++y) {
		for (int x{ 0 }; x < alphabet[y].size(); ++x) {
			std::string ch{ alphabet[y][x] };
			if (alphabet[y][x] == '\n') ch = "RETURN";
			if (alphabet[y][x] == '\0') ch = "DONE";
			text = GameObject::Create(ch)->AddComponent<TextObject>();
			text->SetText(ch, ResourceManager::GetInstance().LoadFont("", 20), 5, TextAllingmentType::Center);
			text->gameObject->SetParent(lettersPrtn);
			text->gameObject->transform->localPosition = Vector2(mapSize / alphabet[0].size() * x, 50.f * y);
			size = text->GetSize();
		}
	}

	letterSquare = GameObject::Create("square")->AddComponent<TextureRenderer>();
	letterSquare->SetTexture("", Vector2(0.5f, 0.5f), 4);
	letterSquare->SetSize(size);
	letterSquare->gameObject->transform->position = scorePrtn->childrenPtr[0]->childrenPtr[0]->transform->position;
	scorePrtn->childrenPtr[0]->childrenPtr[0]->GetComponent<TextObject>()->SetColor(SDL_Color(0,0,0,255));
	scorePrtn->SetActive(false);
	letterSquare->gameObject->SetActive(false);

	InputManager::GetInstance().RegisterCommand(new MenuCommand(gameObject, SDLK_w, MenuControl::up));
	InputManager::GetInstance().RegisterCommand(new MenuCommand(gameObject, SDLK_s, MenuControl::down));
	InputManager::GetInstance().RegisterCommand(new MenuCommand(gameObject, SDLK_SPACE, MenuControl::select));
	InputManager::GetInstance().RegisterCommand(new MenuCommand(gameObject, GamePad::DPad_Up, MenuControl::up));
	InputManager::GetInstance().RegisterCommand(new MenuCommand(gameObject, GamePad::DPad_Down, MenuControl::down));
	InputManager::GetInstance().RegisterCommand(new MenuCommand(gameObject, GamePad::A, MenuControl::select));

	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_w, MenuControl::up));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_s, MenuControl::down));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_a, MenuControl::left));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_d, MenuControl::right));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_SPACE, MenuControl::select));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, SDLK_BACKSPACE, MenuControl::cancel));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::DPad_Up, MenuControl::up));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::DPad_Down, MenuControl::down));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::DPad_Left, MenuControl::left));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::DPad_Right, MenuControl::right));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::A, MenuControl::select));
	InputManager::GetInstance().RegisterCommand(new ScoreCommand(gameObject, GamePad::B, MenuControl::cancel));

}

void GameManager::Init()
{
	for (auto& e : enemies) {
		if (e != nullptr) GameObject::Delete(e->gameObject);
	}
	enemies.clear();
	for (auto& b : bullets) {
		GameObject::Delete(b->gameObject);
	}
	bullets.clear();

	menuPrnt->SetActive(false);

	mapCreator.LoadMap("Map" + std::to_string((lvl-1) % 3 + 1) + ".data", mapSize); //

	freePosList.clear();
	for (int y{1}; y < GRID_SIZE; y++) {
		for (int x{1}; x < GRID_SIZE; x++) {
			if (!CheckColliders(GetPosInGridInternal(x, y), 5)) {
				freePosList.emplace_back(GetPosInGridInternal(x, y));
			}
		}
	}

	//Enemy* testEnemy = GameObject::Create("test")->AddComponent<Enemy>();
	//testEnemy->gameObject->transform->position = Vector2(-mapSize / 2, (-mapSize + TOP_OFFSET) / 2) + 
	//	Vector2(mapSize / GRID_SIZE * 17, mapSize / GRID_SIZE * 1);
	//enemies.emplace_back(testEnemy);

	switch (mode)
	{
	case GameMode::Solo:
		InitSolo();
		break;
	case GameMode::Duo:
		InitCoop();
		break;
	case GameMode::Versus:
		InitVS();
		break;
	default:
		InitSolo();
		break;
	}
}

void GameManager::InitSolo()
{
	if (player == nullptr) {
		player = GameObject::Create("Player")->AddComponent<Player>();
		player->Init(0);
	}
	player->gameObject->transform->position = freePosList[Random::GetRandom(0, freePosList.size())];
	player->gameObject->SetActive(true);
	player->Spawn();

	SpawnEnemies();
}

void GameManager::InitCoop()
{
	if (player == nullptr) {
		player = GameObject::Create("Player")->AddComponent<Player>();
		player->Init(0);
	}
	if (player2 == nullptr) {
		player2 = GameObject::Create("Player2")->AddComponent<Player>();
		player2->Init(1);
	}

	player->gameObject->transform->position = freePosList[Random::GetRandom(0, freePosList.size())];
	player->gameObject->SetActive(true);
	player->Spawn();

	player2->gameObject->transform->position = freePosList[Random::GetRandom(0, freePosList.size())];
	player2->gameObject->SetActive(true);
	player2->Spawn();

	SpawnEnemies();
}

void GameManager::InitVS()
{
	if (player == nullptr) {
		player = GameObject::Create("Player")->AddComponent<Player>();
		player->Init(0);
	}
	if (player2 == nullptr) {
		player2 = GameObject::Create("Player2")->AddComponent<Player>();
		player2->Init(1);
	}

	player->gameObject->transform->position = freePosList[Random::GetRandom(0, freePosList.size())];
	player->gameObject->SetActive(true);
	player->Spawn();

	player2->gameObject->transform->position = freePosList[Random::GetRandom(0, freePosList.size())];
	player2->gameObject->SetActive(true);
	player2->Spawn();
}

void GameManager::ShowMenu()
{
	scorePrtn->SetActive(false);
	letterSquare->gameObject->SetActive(false);
	menuPrnt->SetActive(true);
}

void GameManager::ShowHighScore()
{
	endScore = player->GetScore();

	for (auto& e : enemies) {
		if (e != nullptr) GameObject::Delete(e->gameObject);
	}
	enemies.clear();
	for (auto& b : bullets) {
		GameObject::Delete(b->gameObject);
	}
	bullets.clear();

	mapCreator.CleanUp();

	blocks.clear();
	freePosList.clear();

	if (player != nullptr) {
		GameObject::Delete(player->gameObject);
		player = nullptr;
	}
	if (player2 != nullptr) {
		GameObject::Delete(player2->gameObject);
		player2 = nullptr;
	}
	for (auto& e : enemies) {
		if (e != nullptr) GameObject::Delete(e->gameObject);
	}
	enemies.clear();

	scorePrtn->SetActive(true);
	letterSquare->gameObject->SetActive(true);

	scorePrtn->childrenPtr[1]->GetComponent<TextObject>()->SetText(scoreName);
	scorePrtn->childrenPtr[2]->GetComponent<TextObject>()->SetText(std::to_string(endScore));

	ReadHighScores();
}

void GameManager::SpawnEnemies()
{
	std::vector<Vector3> spawnPos{};

	for (auto& p : freePosList) {
		if (Vector3().Distance(player->gameObject->transform->GetPosition(), p) >= 200) {
			spawnPos.emplace_back(p);
		}
	}

	const int spawnCount{ static_cast<int>(std::floorf(std::sqrtf(lvl * 2.f))) };

	for (int idx{ 0 }; idx < spawnCount; ++idx) {
		size_t rnd = Random::GetRandom(0, spawnPos.size()-1);

		if (Random::GetRandom(0,1) == 0)
		{
			TankEnemy* enemy = GameObject::Create("enemy" + std::to_string(idx))->AddComponent<TankEnemy>();
			enemy->gameObject->transform->position = spawnPos[rnd];
			enemies.emplace_back(enemy);
		}
		else {
			MeleeEnemy* enemy = GameObject::Create("enemy" + std::to_string(idx))->AddComponent<MeleeEnemy>();
			enemy->gameObject->transform->position = spawnPos[rnd];
			enemies.emplace_back(enemy);
		}


		spawnPos.erase(spawnPos.begin() + rnd);
	}
}

void GameManager::Update()
{
	if (dieTimer > 0) {
		dieTimer -= Time::deltaTime;
		if (dieTimer <= 0) {
			if (player->GetLives() <= 0) {
				ShowHighScore();
			}
			else {
				for (auto& e : enemies) {
					if (e != nullptr) GameObject::Delete(e->gameObject);
				}
				enemies.clear();
				for (auto& b : bullets) {
					GameObject::Delete(b->gameObject);
				}
				bullets.clear();

				switch (mode)
				{
				case GameMode::Solo:
					InitSolo();
					break;
				case GameMode::Duo:
					InitCoop();
					break;
				case GameMode::Versus:
					InitVS();
					break;
				default:
					InitSolo();
					break;
				}
			}
		}
	}
	if (InputManager::GetInstance().KeyDown(SDLK_F1)) {
		for (auto& e : enemies) {
			GameObject::Delete(e->gameObject);
		}
		enemies.clear();
		lvl++;

		Init();
	}
	if (InputManager::GetInstance().KeyDown(SDLK_F2)) {
		if (ServiceLocator().GetAudioService().GetVolume() != 0) {
			ServiceLocator().GetAudioService().SetVolume(0);
		}
		else {
			ServiceLocator().GetAudioService().SetVolume(1);

		}
	}
	if (InputManager::GetInstance().KeyDown(SDLK_F3)) {
		player->TakeDamage(3);
	}
}

void GameManager::SetMode(int offset)
{
	menuPrnt->childrenPtr[0]->childrenPtr[static_cast<int>(mode)]->childrenPtr[0]->SetActive(false);

	int modeInt = static_cast<int>(mode) + offset;
	if (modeInt < 0) modeInt = static_cast<int>(GameMode::Versus);
	if (modeInt > static_cast<int>(GameMode::Versus)) modeInt = static_cast<int>(GameMode::Solo);
	mode = static_cast<GameMode>(modeInt);

	menuPrnt->childrenPtr[0]->childrenPtr[modeInt]->childrenPtr[0]->SetActive(true);
}

Vector3 GameManager::GetPosInGridInternal(int x, int y)
{
	return Vector2(-mapSize / 2, (-mapSize + TOP_OFFSET) / 2) + Vector3(mapSize / GRID_SIZE * x, mapSize / GRID_SIZE * y, 0);
}

void GameManager::MoveToLetter(int x, int y)
{
	scorePrtn->childrenPtr[0]->childrenPtr[letterIndex]->GetComponent<TextObject>()->SetColor(SDL_Color(255, 255, 255, 255));
	int index{letterIndex};
	index += x;
	index += static_cast<int>(alphabet[0].size()) * y;

	if (index < 0) {
		if (x != 0) index = (static_cast<int>(scorePrtn->childrenPtr[0]->childrenPtr.size()) - 1);
		if (y != 0) index += static_cast<int>(alphabet[0].size()) * static_cast<int>(alphabet.size());
		if (index >= scorePrtn->childrenPtr[0]->childrenPtr.size()) index -= static_cast<int>(alphabet[0].size());
	}
	if (index >= scorePrtn->childrenPtr[0]->childrenPtr.size()) {
		if (x != 0) index = 0;
		if (y != 0) index = (index % static_cast<int>(alphabet[0].size()));
	}

	letterIndex = static_cast<uint8_t>(index);

	letterSquare->gameObject->transform->position = scorePrtn->childrenPtr[0]->childrenPtr[letterIndex]->transform->position;
	TextObject* t = scorePrtn->childrenPtr[0]->childrenPtr[letterIndex]->GetComponent<TextObject>();
	t->SetColor(SDL_Color(0, 0, 0, 255));
	letterSquare->SetSize(t->GetSize());

}

void GameManager::SelectLetter()
{
	char ch = alphabet[letterIndex / alphabet[0].size()][letterIndex % alphabet[0].size()];
	if (ch == '\n') {
		RemoveLetter();
		return;
	}
	if (ch == '\0') {
		WriteHighScore();
		ShowMenu();
		return;
	}
	if (scoreName.size() > 5) return;
	scoreName += ch;

	scorePrtn->childrenPtr[1]->GetComponent<TextObject>()->SetText(scoreName);
}

void GameManager::RemoveLetter()
{
	if (scoreName.size() == 0) return;
	scoreName.erase(scoreName.end()-1);
	scorePrtn->childrenPtr[1]->GetComponent<TextObject>()->SetText(scoreName);
}

Vector3 GameManager::GetPosInGrid(int x, int y)
{
	Vector3 pos = GetPosInGridInternal(x,y);
	
	int index{ 0 }, closestIndex{ 0 };
	for (auto& p : freePosList) {
		if (Vector3::Distance(pos, p) <= Vector3::Distance(pos, freePosList[closestIndex])) {
			closestIndex = index;
		}
		++index;
	}
	return freePosList[closestIndex];
}

void GameManager::GetGridPos(const Vector3& pos, int& x, int& y)
{
	x = static_cast<int>(std::ceilf((pos.x + (mapSize) / 2) / (mapSize / 18)));
	y = static_cast<int>(std::ceilf((pos.y + (mapSize-TOP_OFFSET) / 2) / (mapSize / 18)));
}

bool GameManager::IsPathInDirectionFree(const Vector3& pos, const Direction dir)
{
	switch (dir)
	{
	case Direction::Right:
		return !CheckColliders(pos + Vector3(mapSize / GRID_SIZE, 0, 0), 5);
		break;
	case Direction::Up:
		return !CheckColliders(pos - Vector3(0, mapSize / GRID_SIZE, 0), 5);
		break;
	case Direction::Left:
		return !CheckColliders(pos - Vector3(mapSize / GRID_SIZE, 0, 0), 5);
		break;
	case Direction::Down:
		return !CheckColliders(pos + Vector3(0, mapSize / GRID_SIZE, 0), 5);
		break;
	default:
		Console::GetInstance().Log("Get Path went wrong: " + pos);
		return false;
		break;
	}
}

Vector3 GameManager::GetRandomPos()
{
	return freePosList[Random::GetRandom(0, freePosList.size()-1)];
}

Vector3 GameManager::GetPlayerPos(int id)
{
	if (id == 0 && player != nullptr) return player->gameObject->transform->position;
	if (id == 1 && player2 != nullptr) return player2->gameObject->transform->position;
	return Vector3();
}

Vector3 GameManager::GetPlayerPos(const Vector3& pos)
{
	if (player != nullptr && player2 == nullptr) return player->gameObject->transform->position;
	if (player->IsDead()) return player2->gameObject->transform->position;
	if (player2->IsDead()) return player->gameObject->transform->position;
	if (Vector2().Distance(pos,player->gameObject->transform->position) <= Vector2().Distance(pos, player2->gameObject->transform->position)) return player->gameObject->transform->position;
	else player2->gameObject->transform->position;
	return Vector3();
}

Player* GameManager::GetPlayer(int id)
{
	if (id == 0) return player;
	else return player2;
}

void GameManager::AddColliders(std::vector<MapBlock*> cols)
{
	blocks = cols;
}

void GameManager::ClearColliders()
{
	blocks.clear();
}

bool GameManager::CheckColliders(const Vector3& pos, const float unitCol)
{
	CollisionDirection dir{};
	for (auto& b : blocks) {
		if (b->CheckCollision(pos, unitCol, dir)) return true;
	}
	return false;
}

bool GameManager::CheckColliders(const Vector3& pos, const float unitCol, CollisionDirection& dir)
{
	for (auto& b : blocks) {
		if (b->CheckCollision(pos, unitCol, dir)) return true;
	}
	return false;
}

Player* GameManager::CheckPlayerColliders(const Vector3& pos, const float unitCol)
{
	if (player->CheckCollision(pos, unitCol)) return player;
	if (player2 != nullptr && player2->CheckCollision(pos, unitCol)) return player2;
	return nullptr;
}

Enemy* GameManager::CheckEnemyColliders(const Vector3& pos, const float unitCol)
{
	for (auto& e : enemies) {
		if (e->CheckCollision(pos, unitCol)) return e;
	}
	return nullptr;
}

void GameManager::CheckDiamond(GameObject* obj)
{
	Vector2 diaPos = Vector2(0,100/2);

	if (Vector2().Distance(diaPos, obj->transform->position) <= 60) {
		obj->transform->position = GetRandomPos();
	}
}

void GameManager::NotifyEnemyDeath(Enemy* en)
{
	enemies.erase(std::find(enemies.begin(), enemies.end(), en));

	if (enemies.size() == 0) {
		lvl++;

		Init();
	}
}

void GameManager::NotifyPlayerDeath(Player*)
{
	if (player2 != nullptr && mode != GameMode::Versus) {
		if (player->IsDead() && player2->IsDead()) {
			ServiceLocator::GetAudioService().PlayAudio("lose.mp3", 0.5f);
			dieTimer = 2.f;
		}
	}
	else {
		ServiceLocator::GetAudioService().PlayAudio("lose.mp3", 0.5f);
		dieTimer = 2.f;
	}
}

void GameManager::RegisterBullet(Bullet* bullet)
{
	bullets.emplace_back(bullet);
}

void GameManager::RemoveBullet(Bullet* bullet)
{
	bullets.erase(std::find(bullets.begin(), bullets.end(), bullet));
}

void GameManager::ReadHighScores()
{
	const std::string PATH{ "../Data/Scores.txt" };
	std::ifstream ifStream{ PATH };

	if (!ifStream) {
		Console().GetInstance().Log("Score Load Failed.");
		return;
	}
	std::string line{};

	while (!ifStream.eof()) {
		std::getline(ifStream, line);
		if (line == "") continue;
		scores.emplace_back(std::pair<std::string, int>(utils::Split(line, ":")[0], std::stoi(utils::Split(line, ":")[1])));
	}

	for (auto& c : scorePrtn->childrenPtr[3]->childrenPtr) {
		GameObject::Delete(c);
	}
	scorePrtn->childrenPtr[3]->childrenPtr.clear();
	for (auto& s : scores) {
		TextObject* text = GameObject::Create("score")->AddComponent<TextObject>();
		text->gameObject->SetParent(scorePrtn->childrenPtr[3]);
		text->gameObject->transform->localPosition = Vector2(0, 30.f * (scorePrtn->childrenPtr[3]->childrenPtr.size() - 1));
		text->SetText(s.first + ": " + std::to_string(s.second), ResourceManager::GetInstance().LoadFont("", 20), 10, TextAllingmentType::Left);
	}
}

void GameManager::WriteHighScore()
{
	const std::string PATH{ "../Data/Scores.txt" };

	scores.emplace_back(std::pair<std::string, int>(scoreName, endScore));
	std::sort(scores.begin(), scores.end(), [&](std::pair<std::string, int>& p1, std::pair<std::string, int>& p2) {
		return p1.second > p2.second;
		});
	if (scores.size() > 10) scores.erase(scores.end() - 1);

	std::ofstream ofstream{};
	ofstream.open(PATH);
	ofstream.clear();
	for (auto& s : scores) {
		ofstream << s.first << ":" << std::to_string(s.second) << '\n';
	}
	ofstream.close();

}

MenuCommand::MenuCommand(GameObject* gameobject, GamePad btn, MenuControl control)
	: Command(gameobject, btn)
{
	ctrl = control;
}

MenuCommand::MenuCommand(GameObject* gameobject, SDL_Keycode keyCode, MenuControl control)
	: Command(gameobject, keyCode)
{
	ctrl = control;

}

void MenuCommand::ExecuteDown()
{
	if (!GameManager::GetInstance().IsInMenu()) return;
	switch (ctrl)
	{
	case MenuControl::up:
		GameManager::GetInstance().SetMode(-1);
		ServiceLocator::GetAudioService().PlayAudio("select.wav", 0.5f);
		break;
	case MenuControl::down:
		GameManager::GetInstance().SetMode(1);
		ServiceLocator::GetAudioService().PlayAudio("select.wav", 0.5f);
		break;
	case MenuControl::select:
		GameManager::GetInstance().Init();
		ServiceLocator::GetAudioService().PlayAudio("button.mp3", 0.5f);
		break;
	default:
		break;
	}
}

ScoreCommand::ScoreCommand(GameObject* gameobject, GamePad btn, MenuControl control)
	: Command(gameobject, btn)

{
	ctrl = control;
}

ScoreCommand::ScoreCommand(GameObject* gameobject, SDL_Keycode keyCode, MenuControl control)
	: Command(gameobject, keyCode)
{
	ctrl = control;
}

void ScoreCommand::ExecuteDown()
{
	if (!GameManager::GetInstance().IsInScore()) return;
	switch (ctrl)
	{
	case MenuControl::up:
		GameManager::GetInstance().MoveToLetter(0, -1);
		break;
	case MenuControl::down:
		GameManager::GetInstance().MoveToLetter(0, 1);
		break;
	case MenuControl::left:
		GameManager::GetInstance().MoveToLetter(-1, 0);
		break;
	case MenuControl::right:
		GameManager::GetInstance().MoveToLetter(1, 0);
		break;
	case MenuControl::select:
		GameManager::GetInstance().SelectLetter();
		break;
	case MenuControl::cancel:
		GameManager::GetInstance().RemoveLetter();
		break;
	default:
		break;
	}
}
