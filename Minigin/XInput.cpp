#define _AMD64_
#include "XInput.h"
#include <Xinput.h>
#include <windows.h>
#pragma comment(lib, "XInput.lib") 

class XInput::XInputImpl {
	XINPUT_STATE state;
public:

	void ProcessInput(Command* command) {
        DWORD dwResult;
        for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
        {
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            dwResult = XInputGetState(i, &state);

            if (dwResult == ERROR_SUCCESS)
            {
                if (state.Gamepad.wButtons & command->GetBtn()) {
                    command->ExecutePressed();
                }
            }
            else
            {

            }
        }
	}
};
XInput::XInput()
{
    xImpl = std::make_unique<XInputImpl>();
}
XInput::~XInput() = default;

void XInput::ProcessInput(Command* command)
{
    xImpl->ProcessInput(command);
}
