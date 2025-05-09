#include "PlayerUI.h"
#include "EventTypes.h"

void PlayerUI::Init(Player* player)
{
	playerPtr = player;
	this->text = gameObject->GetComponent<TextObject>();
}

void PlayerUI::Update(Event e, EventArgs args)
{
	EventType eventT = static_cast<EventType>(e);
	if (eventT == EventType::Player_Hurt || eventT == EventType::Score_Change) {
		if (playerPtr != nullptr) {
			text->SetText("#lives: " + std::to_string(playerPtr->GetHealth()) + "\n" + std::to_string(playerPtr->GetScore()));
		}
	}
}

//from event manager
void PlayerUI::HandleEvent(Event e, EventArgs args)
{
	EventType eventT = static_cast<EventType>(e);
	if (eventT == EventType::Player_Hurt || eventT == EventType::Score_Change) {
		if (playerPtr != nullptr) {
			text->SetText("#lives: " + std::to_string(playerPtr->GetHealth()) + "\n" + std::to_string(playerPtr->GetScore()));
		}
	}
}
