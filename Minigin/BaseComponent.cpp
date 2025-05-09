#include "BaseComponent.h"
#include "GameObject.h" //...2 hrs... I AM GOING TO KILL SOMEONE!!!
#include "Time.h"

BaseComponent::BaseComponent():
    gameObject{},
    checkGO{},
    classType{}
{
    //Start();
}

BaseComponent::~BaseComponent() {
}

std::function<void()> BaseComponent::GetFunction(std::string name)
{
    //if (funcMap.Exist(name)) return funcMap[name];
    return NULL;
}

std::string BaseComponent::GetType()
{
    return classType;
}

void BaseComponent::CheckGO()
{
    //assert(checkGO == gameObject);
    if (gameObject != checkGO) gameObject = checkGO;
}

void BaseComponent::Initialize(GameObject* objPtr, std::string type)
{
    gameObject = objPtr;
    checkGO = objPtr;
    classType = type;
}
