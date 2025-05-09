#pragma once
#include "Singleton.h"
//#include "EventTypes.h"
#include "EventHandler.h"

class EventManager : public Singleton<EventManager>
{
public:
	void SendMessage(Event e, EventArgs eArgs = EventArgs());
	template <typename T>
	void SendMessage(T e, EventArgs eArgs = EventArgs())
	{
		eventQueue.emplace_back(std::pair<Event, EventArgs>(static_cast<Event>(static_cast<int>(e)), eArgs));
	}
	void Update(); // ???

	void AddListener(GameObject* obj, IEventHandler* handler, Event eventIDs);
	void AddListener(GameObject* obj, IEventHandler* handler, std::vector<Event> eventIDs);
	template <typename T>
	void AddListener(GameObject* obj, IEventHandler* handler, T eventIDs)
	{
		handler->Init(obj, static_cast<Event>(static_cast<int>(eventIDs)));
		eventHandlers.emplace_back(handler);
	}
	template <typename T>
	void AddListener(GameObject* obj, IEventHandler* handler, std::vector<T> eventIDs)
	{
		//EventHandler* event = new Class(eventID, (args));
		std::vector<Event> events{};
		for (auto& e : eventIDs) {
			events.emplace_back(static_cast<Event>(static_cast<int>(e)));
		}
		handler->Init(obj, events);
		eventHandlers.emplace_back(handler);
		//return event; 
	}

	void RemoveListener(IEventHandler* handler);
	void RemoveListener(GameObject* obj);

private:
	std::vector<IEventHandler*> eventHandlers{};
	std::vector<std::pair<Event, EventArgs>> eventQueue{};

	bool PullQueue(Event& e, EventArgs& args);
};

