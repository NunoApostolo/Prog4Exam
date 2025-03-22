#pragma once
#include "Singleton.h"
#include "EventTypes.h"
#include "EventHandler.h"

class EventManager : public Singleton<EventManager>
{
public:
	void SendMessage(Event e, EventArgs eArgs = EventArgs());
	void Update(); // ???

	void AddListener(IEventHandler* handler, Event eventIDs) {
		//EventHandler* event = new Class(eventID, (args));
		handler->Init(eventIDs);
		eventHandlers.emplace_back(handler);
		//return event; 
	}


private:
	std::vector<IEventHandler*> eventHandlers{};
	std::vector<std::pair<Event, EventArgs>> eventQueue{};

	bool PullQueue(Event& e, EventArgs& args);
};

