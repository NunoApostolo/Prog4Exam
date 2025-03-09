#include "Command.h"

void Command::Register(SDL_KeyCode key, std::function<void()> function, std::function<bool(SDL_KeyCode)> keyFunction)
{
    this->key = key;
    this->func = function;
    this->keyFunc = keyFunction;
}

void Command::Execute()
{
    func();
}

SDL_KeyCode Command::GetKey()
{
    return key;
}
