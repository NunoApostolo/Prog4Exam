#pragma once
#include "IMouseHandler.h"
#include "TextObject.h"

class Button : public BaseComponent, public IMouseHandler
{
public:
	void Awake() override;
	void Start() override;

	std::function<void()> onMouseClick{ nullptr };

	std::shared_ptr<Texture2D> clickTex;
	std::shared_ptr<Texture2D> enterTex;
	std::shared_ptr<Texture2D> tex;

	void SetText(std::string text);
protected:
	void OnPointerEnter(PointerData);
	void OnPointerExit(PointerData);
	void OnPointerDown(PointerData);
	void OnPointerUp(PointerData);
	TextObject* text;
	bool mouseDown{ false };

};

