#pragma once
#include "Observer.h"
#include "TextObject.h"
#include "EventHandler.h"
#include "Player.h"

// test both types
class PlayerUI :public BaseComponent, public Observer, public IEventHandler
{
public:
	PlayerUI(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}
	void Init(Player* player);

	//test both methods
	void Update(Event event, EventArgs args) override;
	void HandleEvent(Event e, EventArgs args) override;

private:
	TextObject* text{nullptr};
	//keep tabs
	Player* playerPtr{ nullptr };
};

