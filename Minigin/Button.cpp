#pragma once
#include "Button.h"
#include "GameObject.h"

void Button::Awake()
{
	if (gameObject->GetComponent<TextureRenderer>() == nullptr) {
		gameObject->AddComponent<TextureRenderer>();
	}
	Init(gameObject);
}

void Button::Start()
{
	if (GetTex()->GetTexture() == nullptr) GetTex()->SetTexture(tex, Vector2(0.5f, 0.5f), gameObject->GetOrder(), GetTex()->GetSize());
}

void Button::SetText(std::string txt)
{
	if (gameObject->GetComponent<TextObject>() == nullptr) {
		text = gameObject->AddComponent<TextObject>();
	}
	if (GetTex()->GetTexture() == nullptr) Start();
	text->SetBounds(GetTex()->GetSize());
	text->SetText(txt, ResourceManager::GetInstance().LoadFont("EncodeSansCompressed-700-Bold.ttf", 20), gameObject->GetOrder()+1);
}

void Button::OnPointerEnter(PointerData)
{
	if (enterTex != nullptr) GetTex()->SetTexture(enterTex);
}

void Button::OnPointerExit(PointerData)
{
	mouseDown = false;
	if (tex != nullptr) GetTex()->SetTexture(tex);
}

void Button::OnPointerDown(PointerData)
{
	mouseDown = true;
	if (clickTex != nullptr) GetTex()->SetTexture(clickTex);
}

void Button::OnPointerUp(PointerData p)
{
	if (tex != nullptr) GetTex()->SetTexture(tex);
	if (p.button == 0 && mouseDown) {
		if (onMouseClick != nullptr) onMouseClick();
	}
	mouseDown = false;
}
