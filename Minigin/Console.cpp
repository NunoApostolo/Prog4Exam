#include "Console.h"

void Console::Log(std::string string)
{
	string += "\n";
	printf(string.c_str());
}
void Console::Log(Vector3 string)
{
	Log("" + string);
}
