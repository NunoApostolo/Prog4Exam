#include "Command.h"

Command::Command(GameObject* gameobject, SDL_Keycode keyCode)
{
    this->gameObj = gameobject;
    this->key = keyCode;
}

SDL_Keycode Command::GetKey()
{
    return key;
}