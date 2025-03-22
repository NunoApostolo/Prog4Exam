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
			if (eventHandlers[idx]->HasEventID(e)) {
				eventHandlers[idx]->HandleEvent(e, args);
			} 
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
