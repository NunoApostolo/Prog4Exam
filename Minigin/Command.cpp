#pragma once
#include "Command.h"

Command::Command(GameObject* gameobject, GamePad button)
{
    this->gameObj = gameobject;
    this->btn = (unsigned long)button;
    isGamePad = true;
}

Command::Command(GameObject* gameobject, SDL_Keycode keyCode)
{
    this->gameObj = gameobject;
    this->key = keyCode;
}

SDL_Keycode Command::GetKey()
{
    return key;
}

unsigned long Command::GetBtn()
{
    return btn;
}

GameObject* Command::GetObj()
{
    return gameObj;
}
