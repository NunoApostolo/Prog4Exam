#include "Command.h"

void Command::Register(SDL_Keycode keycode, std::function<void()> function, std::function<bool(SDL_Keycode)> keyFunction)
{
    this->key = keycode;
    this->func = function;
    this->keyFunc = keyFunction;
}

void Command::Register(WORD btncode, std::function<void()> function, std::function<bool(SDL_Keycode)> keyFunction)
{
    this->btn = btncode;
    this->func = function;
    this->keyFunc = keyFunction;
}

void Command::Execute()
{
    if (func != NULL) func();
}

SDL_Keycode Command::GetKey()
{
    return key;
}

WORD Command::GetBtn()
{
    return btn;
}
