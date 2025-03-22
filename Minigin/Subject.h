#pragma once
#include "Observer.h"

class Subject
{
public:
	void RegisterObserver(Observer* observer);
	template <typename T>
	void RemoveObserver() {
		std::remove(observers.begin(), observers.end(),
			std::find_if(observers.begin(), observers.end(), [&](Observer* obs) {return typeid(obs) == typeid(T); }));
	}

	virtual void Notify(Event event, EventArgs args);

private:
	std::vector<Observer*> observers{};
};

