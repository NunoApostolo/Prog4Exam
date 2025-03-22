#include "Subject.h"

void Subject::RegisterObserver(Observer* observer)
{
	observers.emplace_back((observer));
}

void Subject::Notify(Event event, EventArgs args)
{
	for (auto& observer : observers) {
		observer->Update(event, args);
	}
}
