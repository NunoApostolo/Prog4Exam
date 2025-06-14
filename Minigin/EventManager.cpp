#include "EventManager.h"
#include <algorithm>

void EventManager::SendMessage(Event e, EventArgs args)
{
	eventQueue.emplace_back(std::pair<Event, EventArgs>(e,args));
}

void EventManager::Update()
{
	Event e;
	EventArgs args;
	while (PullQueue(e, args))
	{
		for (size_t idx{ 0 }; idx < eventHandlers.size(); idx++) {

			if (eventHandlers[idx]->HasEventID(static_cast<uint16_t>(e))) {
				if (e >= Event::End) {
					eventHandlers[idx]->HandleEvent(static_cast<Event>(static_cast<uint16_t>(e)), args);
				}
				else {
					eventHandlers[idx]->HandleEvent(e, args);
				}
			}
		}
	}
}

void EventManager::AddListener(GameObject* obj, IEventHandler* handler, Event eventIDs)
{
	//EventHandler* event = new Class(eventID, (args));
	handler->Init(obj, eventIDs);
	eventHandlers.emplace_back(handler);
	//return event; 
}

void EventManager::AddListener(GameObject* obj, IEventHandler* handler, std::vector<Event> eventIDs)
{
	//EventHandler* event = new Class(eventID, (args));
	handler->Init(obj, eventIDs);
	eventHandlers.emplace_back(handler);
	//return event; 
}

void EventManager::RemoveListener(IEventHandler* handler)
{
	eventHandlers.erase(std::find(eventHandlers.begin(), eventHandlers.end(), handler));
}

void EventManager::RemoveListener(GameObject* obj)
{
	for (size_t i{ 0 }; i < eventHandlers.size(); ++i) {
		if (eventHandlers[i]->GetObj() == obj) {
			eventHandlers.erase(eventHandlers.begin() + i);
			RemoveListener(obj);
			return;
		}
	}
}

bool EventManager::PullQueue(Event& e, EventArgs& args)
{
	if (eventQueue.size() > 0) {
		e = eventQueue[0].first;
		args = eventQueue[0].second;
		eventQueue.erase(eventQueue.begin(), eventQueue.begin()+1);
		return true;
	}
	return false;
}
