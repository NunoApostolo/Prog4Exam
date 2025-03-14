#pragma once
#include <memory>
#include "Singleton.h"
#include "Command.h"

class XInput : public Singleton<XInput>
{
public:
	XInput();
	~XInput();

	void ProcessInput(Command* command);

private:
	class XInputImpl;
	std::unique_ptr<XInputImpl> xImpl;
};

