#include "PlayerUI.h"
#include "EventTypes.h"

void PlayerUI::Init(Player* player)
{
	playerPtr = player;
	this->scoreText = GameObject::Create("Score Text")->AddComponent<TextObject>();
	scoreText->gameObject->SetParent(gameObject);
	scoreText->gameObject->transform->localPosition = Vector2(0, -15);
	scoreText->SetText("Score:", ResourceManager::GetInstance().LoadFont("", 30), 20, TextAllingmentType::Left);
	this->livesText = GameObject::Create("Score Text")->AddComponent<TextObject>();
	livesText->gameObject->SetParent(gameObject);
	livesText->gameObject->transform->localPosition = Vector2(0, 15);
	livesText->SetText("Lives:", ResourceManager::GetInstance().LoadFont("", 30), 20, TextAllingmentType::Left);

	scoreText->SetText("Score: " + std::to_string(playerPtr->GetScore()));
	livesText->SetText("Lives: " + std::to_string(playerPtr->GetLives()));
}

void PlayerUI::Update(Event e, EventArgs args)
{
	EventType eventT = static_cast<EventType>(e);
	if (eventT == EventType::Player_Hurt || eventT == EventType::Score_Change) {
		if (playerPtr != nullptr) {
			scoreText->SetText("Score: " + std::to_string(playerPtr->GetScore()));
			livesText->SetText("Lives: " + std::to_string(playerPtr->GetLives()));
		}
	}
}

//from event manager
void PlayerUI::HandleEvent(Event e, EventArgs args)
{
	EventType eventT = static_cast<EventType>(e);
	if (eventT == EventType::Player_Hurt || eventT == EventType::Score_Change) {
		if (playerPtr != nullptr) {
			scoreText->SetText("Score: " + std::to_string(playerPtr->GetScore()));
			livesText->SetText("Lives: " + std::to_string(playerPtr->GetLives()));
		}
	}
}
