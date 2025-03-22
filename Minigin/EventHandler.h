#pragma once
#include "EventTypes.h"
#include "GameObject.h"

class IEventHandler {
public:
	IEventHandler() :eventIDs{} {}
	IEventHandler(Event event) :eventIDs{event} {}
	void Init(Event event) { eventIDs.emplace_back(event); }
	virtual ~IEventHandler() {}
	virtual void HandleEvent(Event e, EventArgs args) = 0; //Event e,

	bool HasEventID(Event e) { return std::find(eventIDs.begin(), eventIDs.end(), e) != eventIDs.end(); }

private:
	std::vector<Event> eventIDs;
};
