#pragma once
#include "Singleton.h"
#include "Vector.h"
#include <iostream>

class Console : public Singleton<Console>
{
public:
	void Log(std::string string);
	void Log(Vector3 string);
	
};

