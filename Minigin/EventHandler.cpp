#pragma once
#include "EventHandler.h"
#include "GameObject.h"

void IEventHandler::Init(GameObject* gameObject, Event event)
{
	obj = gameObject; 
	eventIDs.emplace_back(static_cast<uint16_t>(event));
}

void IEventHandler::Init(GameObject* gameObject, std::vector<Event> event)
{
	obj = gameObject; 
	for (auto& e : event) {
		eventIDs.emplace_back(static_cast<uint16_t>(e));
	}
}

bool IEventHandler::HasEventID(Event e)
{
	return std::find(eventIDs.begin(), eventIDs.end(), static_cast<uint16_t>(e)) != eventIDs.end();
}

bool IEventHandler::HasEventID(uint16_t e)
{
	return std::find(eventIDs.begin(), eventIDs.end(), e) != eventIDs.end();
}
