#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
#include "TextObject.h"

class FpsCounter : public BaseComponent
{
public:
	FpsCounter(GameObject* objPtr, std::string type) : BaseComponent(objPtr, type) {}

	void Start() override;
	void Update() override;
	void Render() const override;

private:
	TextObject* text{};
};

