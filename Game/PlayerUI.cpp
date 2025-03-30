#include "PlayerUI.h"

void PlayerUI::Init(Player* player)
{
	playerPtr = player;
	this->text = gameObject->GetComponent<TextObject>();
}

void PlayerUI::Update(Event event, EventArgs args)
{
	if (event == Event::Player_Hurt || event == Event::Score_Change) {
		if (playerPtr != nullptr) {
			text->SetText("#lives: " + std::to_string(playerPtr->GetHealth()) + "\n" + std::to_string(playerPtr->GetScore()));
		}
	}
}

//from event manager
void PlayerUI::HandleEvent(Event e, EventArgs args)
{
	if (e == Event::Player_Hurt || e == Event::Score_Change) {
		if (playerPtr != nullptr) {
			text->SetText("#lives: " + std::to_string(playerPtr->GetHealth()) + "\n" + std::to_string(playerPtr->GetScore()));
		}
	}
}
