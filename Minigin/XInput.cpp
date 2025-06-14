//#if _WIN32
//#define _X86_
//#else
#pragma once
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
#define XUSER_MAX 2 // uhh
#include "Console.h"

class XInput::XInputImpl {
    XINPUT_STATE state{}; // 4
    XINPUT_STATE prevState{};

    std::vector<XINPUT_STATE> states{ std::vector<XINPUT_STATE>(XUSER_MAX) };
    std::vector<XINPUT_STATE> prevStates{ std::vector<XINPUT_STATE> (XUSER_MAX)};

    int userCount{ 0 };
public:
    void ProcessInput() {
        userCount = 0;
        DWORD dwResult;
        for (DWORD i = 0; i < XUSER_MAX; i++)
        {
            CopyMemory(&prevStates[i], &states[i], sizeof(XINPUT_STATE));
            ZeroMemory(&state, sizeof(XINPUT_STATE));
            dwResult = XInputGetState(i, &state);

            if (dwResult == ERROR_SUCCESS)
            {
                userCount++;
                states[i] = state;
            }
            else
            {

            }
        }
    }
    void ProcessCommand(Command* command) {
        for (size_t idx{ 0 }; idx < states.size(); ++idx) {
            if (command->GetUser() != static_cast<int>(idx)) continue;
            if ((states[idx].Gamepad.wButtons ^ prevStates[idx].Gamepad.wButtons) & command->GetBtn() && states[idx].Gamepad.wButtons & command->GetBtn()) {
                command->ExecuteDown();
            }
            if (states[idx].Gamepad.wButtons & command->GetBtn()) {
                command->ExecutePressed();
            }
            if ((states[idx].Gamepad.wButtons ^ prevStates[idx].Gamepad.wButtons) & command->GetBtn() && prevStates[idx].Gamepad.wButtons & command->GetBtn()) {
                command->ExecuteUp();
            }
            if (command->GetBtn() & static_cast<unsigned long>(GamePad::Left_Trigger) && states[idx].Gamepad.bLeftTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
                && prevStates[idx].Gamepad.bLeftTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
                command->ExecuteDown();
            }
            if (command->GetBtn() & static_cast<unsigned long>(GamePad::Right_Trigger) && states[idx].Gamepad.bRightTrigger >= XINPUT_GAMEPAD_TRIGGER_THRESHOLD
                && prevStates[idx].Gamepad.bRightTrigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD) {
                command->ExecuteDown();
            }

            const float MAX{ 32767.f };

            Vector2 pos = Vector2(states[idx].Gamepad.sThumbLX, states[idx].Gamepad.sThumbLY);
            pos.Normalize();
            pos.y = pos.y * -1;
            if (Vector2().Length(pos) >= 0.7f) {
                command->MoveLThumb(pos);
            }
            pos = Vector2(states[idx].Gamepad.sThumbRX / MAX, states[idx].Gamepad.sThumbRY / MAX);
            pos.Normalize();
            pos.y = pos.y * -1;
            if (Vector2().Length(pos) >= 0.7f) {
                command->MoveRThumb(pos);
            }
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
