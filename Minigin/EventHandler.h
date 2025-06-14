#pragma once
#include "EngineEventTypes.h"

class GameObject;
class IEventHandler {
public:
	IEventHandler() :eventIDs{}, obj{ nullptr } {}
	//IEventHandler(GameObject* obj, Event event) : obj{obj}, eventIDs{event} {}
	void Init(GameObject* gameObject, Event event);
	void Init(GameObject* gameObject, std::vector<Event> event);
	virtual ~IEventHandler() {}
	virtual void HandleEvent(Event e, EventArgs args) = 0; //Event e,

	bool HasEventID(Event e);
	bool HasEventID(uint16_t e);
	//bool HasEventID(Event e) { return std::find(eventIDs.begin(), eventIDs.end(), e) != eventIDs.end(); }
	GameObject* GetObj() { return obj; }

private:
	std::vector<uint16_t> eventIDs;
	GameObject* obj;
};
