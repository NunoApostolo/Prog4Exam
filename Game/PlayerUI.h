#pragma once
#include "Observer.h"
#include "TextObject.h"
#include "EventHandler.h"
#include "Player.h"

// test both types
class PlayerUI :public BaseComponent, public Observer, public IEventHandler
{
public:
	void Init(Player* player);

	//test both methods
	void Update(Event event, EventArgs args) override;
	void HandleEvent(Event e, EventArgs args) override;

private:
	TextObject* scoreText{nullptr};
	TextObject* livesText{ nullptr };
	//keep tabs
	Player* playerPtr{ nullptr };
};

