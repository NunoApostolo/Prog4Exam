#pragma once
#include "FpsCounter.h"



void FpsCounter::Start()
{
	text = gameObject->GetComponent<TextObject>();
}

void FpsCounter::Update()
{
	text->SetText(std::to_string(1 / Time::deltaTime));
}

void FpsCounter::Render() const
{

}
