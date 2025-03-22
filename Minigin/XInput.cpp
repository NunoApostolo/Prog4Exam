//#if _WIN32
//#define _X86_
//#else
//#define _AMD64_
//#endif 

#if _WIN32 || _WIN64
#if _WIN64
#define _AMD64_
#else
#define _X86_
#endif
#endif

#include "XInput.h"
#include <Xinput.h>
#include <windows.h>
#pragma comment(lib, "XInput.lib")
#define XUSER_MAX 1 // uhh

class XInput::XInputImpl {
	XINPUT_STATE state; // 4
	XINPUT_STATE prevState;
public:
    void ProcessInput() {
        DWORD dwResult;
        for (DWORD i = 0; i < XUSER_MAX; i++)
        {
            CopyMemory(&prevState, &state, sizeof(XINPUT_STATE));
            ZeroMemory(&state, sizeof(XINPUT_STATE));
            dwResult = XInputGetState(i, &state);

            if (dwResult == ERROR_SUCCESS)
            {
                // only supports one controller...
            }
            else
            {

            }
        }
    }
    void ProcessCommand(Command* command) {
        if ((state.Gamepad.wButtons ^ prevState.Gamepad.wButtons) & command->GetBtn() && state.Gamepad.wButtons & command->GetBtn()) {
            command->ExecuteDown();
        }
        if (state.Gamepad.wButtons & command->GetBtn()) {
            command->ExecutePressed();
        }
        if ((state.Gamepad.wButtons ^ prevState.Gamepad.wButtons) & command->GetBtn() && prevState.Gamepad.wButtons & command->GetBtn()) {
            command->ExecuteUp();
        }
    }
};
XInput::XInput()
{
    xImpl = std::make_unique<XInputImpl>();
}
XInput::~XInput() = default;

void XInput::ProcessInput()
{
    xImpl->ProcessInput();
}

void XInput::ProcessCommand(Command* command)
{
    xImpl->ProcessCommand(command);
}
