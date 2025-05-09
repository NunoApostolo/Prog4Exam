#pragma once
#include "EventManager.h"
#include "TextureRenderer.h"
#include "EventHandler.h"

// button (-1 = none, 0 = left, 1 = right)
struct PointerData {
	int button;
	Vector2 pos;
};

class IMouseHandler : public IEventHandler
{
public:
	void Init(GameObject* gameobject);
	void HandleEvent(Event e, EventArgs args) override;

	TextureRenderer* GetTex() { return tex; }
protected:
	// don't feel like separating them for pure virtual functions
	virtual void OnPointerEnter(PointerData) {}
	virtual void OnPointerExit(PointerData) {}
	virtual void OnPointerDown(PointerData) {}
	virtual void OnPointerUp(PointerData) {}

	virtual bool IsPointInTex(Vector2 pos, Vector2& pointerPos);
private:

	TextureRenderer* tex{nullptr};
	bool isMouseOver{ false };
};

