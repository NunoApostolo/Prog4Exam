#pragma once
#include "GameObject.h"
#include "EventTypes.h"

class Observer
{
public:
	virtual ~Observer() {};
	virtual void Update(Event event, EventArgs args) = 0;

private:

};

